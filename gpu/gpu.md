# The GPU layer

Same shape as [`window/`](../window/window.md): every function each graphics API
exposes, declared in Caustic, with a portable abstraction built on top of those
bindings rather than on FFI directly.

Three things live here, and the middle one is what makes this a layer rather
than a binding dump:

```
gpu/
  vk/  gl/  d3d/     raw bindings — everything the API exposes
  device            our abstraction: a portable GPU, in the shape of wgpu
  software/         a backend of that abstraction, implemented by us
```

**`gpu.open()` is ours and it is not a renderer.** It is a device: buffers,
textures, samplers, pipelines, command submission, and compute. What wgpu is to
Vulkan, this is to the bindings beneath it — one API over several backends, at
the level the hardware actually works.

`render/` is a *client* of this, the way a game framework is a client of wgpu.
It is not a peer and it does not have backends of its own: everything portable
about reaching hardware is settled here, and `render/` only decides what to draw.

**Software is a backend here, not in `render/`.** It is another implementation
of the same device — it fills buffers, runs pipelines and dispatches compute, in
software. That is what makes a program's choice of backend invisible to
everything above.

It is six times the size of the window layer, and three of its problems have no
counterpart there.

---

## The surface to be bound

Measured, not estimated:

| API | Functions | Types | Notes |
|-----|-----------|-------|-------|
| **Vulkan** | **1719 commands** | **1606 structs**, 330 enum groups, 61 handles | 665 extensions; `vk.xml` registry is 3 MB |
| **OpenGL** | **3470** exported by libGL | — | `glext.h` alone declares 5283 prototypes |
| **Direct3D 12** | 1572 methods | across **62 COM interfaces** | `d3d12.h` is 1.2 MB |
| **Direct3D 11** | 569 methods | 41 interfaces | |
| **DXGI** | 170 methods | 14 interfaces | swapchain and adapter enumeration for both |
| **EGL / WGL** | ~90 | — | context and surface creation |

Around **7500 functions and 2500 types** for full coverage. Hand-writing that is
not a plan; see *Generators* below.

---

## Vulkan is not linked the way it looks

The important discovery, and it works in the project's favour.

`libvulkan.so.1` exports 269 of Vulkan's 1719 commands. Everything else —
including every extension, and the swapchain is an extension — is fetched at run
time:

```
vkGetInstanceProcAddr(instance, "vkCreateSwapchainKHR")  -> function pointer
vkGetDeviceProcAddr(device, "vkQueueSubmit")             -> function pointer
```

So a Vulkan binding is not 1719 `extern` declarations. It is **one** extern —
`vkGetInstanceProcAddr` — plus a dispatch table of function pointers filled in at
run time and called through `fn_ptr`/`call`, which Caustic already has.

Three consequences:

- **A binary can carry Vulkan with almost no link-time dependency.** Resolve the
  loader by name and derive the rest. This is what `volk` does for C, and it is
  the difference between `DT_NEEDED libvulkan.so.1` and nothing at all.
- Extensions cost nothing structurally: an extension command is another entry in
  the same table.
- **Dispatch is per-instance and per-device.** Device-level commands fetched
  through `vkGetInstanceProcAddr` go through a loader trampoline that costs a
  lookup on every call; fetched through `vkGetDeviceProcAddr` they are direct.
  A table that does not make that distinction quietly gives up performance on
  every draw call.

---

## Direct3D is COM, which is a different binding problem entirely

Vulkan and OpenGL are flat C functions. Direct3D is not: `ID3D12Device`,
`IDXGIFactory`, `ID3D12GraphicsCommandList` are **COM interfaces**, and calling a
method means walking a vtable:

```
object            -> [0] vtable pointer
vtable            -> [n] method pointer      (n = the method's slot)
call(method, object, args...)                (object is the implicit first arg)
```

Caustic can do this today with `fn_ptr` and `call` — the mechanism exists. What
the binding has to carry is **the slot number of every method**, and a slot is
determined by declaration order in the interface, including everything inherited
from `IUnknown` (`QueryInterface`, `AddRef`, `Release` occupy slots 0, 1, 2).

Get a slot wrong and the call lands on a different method with a different
signature. There is no diagnostic for that — it is the same class of silent
failure as a wrong struct offset, at 2300 opportunities across d3d12, d3d11 and
dxgi. Another reason the tables are generated rather than typed.

COM also brings reference counting: every interface is `AddRef`/`Release`, and
the abstraction has to decide whether it owns those calls or exposes them.

---

## Registries: the same story as Wayland, and it is the standard practice

Vulkan and OpenGL both publish **machine-readable registries** — `vk.xml` (3 MB,
present on this machine at `/usr/share/vulkan/registry/`) and `gl.xml` (from
Khronos; not installed here). Every language's Vulkan and OpenGL bindings are
generated from them. Nobody types these by hand, including the C headers
themselves — `vulkan_core.h` is generated output.

That makes the generator story stronger here than in the window layer, not
weaker: the source of truth is designed to be consumed by a program.

- **`vk.xml`** carries commands, structs, enums, handles, extension membership,
  and — importantly — which commands are instance-level and which are
  device-level, which is exactly what the dispatch table split needs.
- **`gl.xml`** carries commands and enums per version and per extension, so a
  binding can be generated for a chosen profile instead of all 3470 at once.
- **Direct3D has no registry**, so it goes through the C header parser. Headers
  are available locally through mingw (`/usr/x86_64-w64-mingw32/include/`) and
  wine, so this is buildable and testable on Linux.

---

## Generators

Three, and two of them are shared with the window layer:

| Generator | Feeds | Shared with |
|-----------|-------|-------------|
| **XML registry → Caustic** | Vulkan, OpenGL | Wayland uses the same idea with a different schema |
| **C header → Caustic** | Direct3D, EGL/WGL, GL fallback | X11, DRM, Win32 |
| **COM vtable extractor** | Direct3D | nothing — this is unique to D3D |

The COM extractor is the one with no precedent elsewhere in the project: it reads
interface declarations and emits, for each method, its vtable slot and signature,
plus the inheritance chain that determines where slots start.

---

## The device abstraction

Device access — buffers, textures, samplers, pipelines, command submission,
compute — and it stops there. It does not know what a mesh is, what a material
is, or what a sprite is; those are [`render/`](../render/render.md)'s words.

```cst
// our way
let is media.gpu.Device as d = media.gpu.open(&win, media.gpu.AUTO);

// name the backend
let is media.gpu.Device as d = media.gpu.open(&win, media.gpu.VULKAN);
let is media.gpu.Device as d = media.gpu.open(&win, media.gpu.SOFTWARE);

// leave the abstraction entirely; the window still supplies the surface
let is *u8 as surface = media.gpu.vk.surface_from(&win);
```

Backends coexist: two devices on different backends can be alive at once. That
requires runtime dispatch through a vtable in `Device` rather than the
compile-time folding the rest of the library uses, which is the trade recorded in
the README — and the reason a program that only ever touches `gpu.vk.*` still
gets everything else stripped by DCE.

The surface comes from the window's native handles, which is why those are public
contract there rather than an implementation detail.

**Compute lives here and nowhere else.** `render/`'s unit of work is a draw;
adding dispatch there would make a pass polymorphic and double its surface for
something inherently explicit — workgroup sizes, storage buffers, barriers.
Wrapping that in a framework loses exactly what makes it useful. A program that
needs compute drops one level, which is what this level is for. Tessellation and
geometry stages are pipeline configuration, so they live where pipelines live:
also here.

The software backend implements all of it, compute included. Nothing is out of
reach for software — it is code, and the constraint is speed rather than
capability.

---

## Shaders

**SPIR-V is the internal currency**, not merely Vulkan's input format. The
finding that settles it: OpenGL takes SPIR-V directly too — `GL_ARB_gl_spirv` has
been core since 4.6, through `glShaderBinary` + `glSpecializeShader`, and 4.6 is
what Mesa reports here.

```
              source
                |            one frontend
              SPIR-V
        /        |        \
   Vulkan    OpenGL 4.6   software
   native    native       must execute it
```

So the choice of input language costs one frontend rather than three backends,
and two of the three consumers need no work at all.

**Decided: SPIR-V comes in precompiled.** A program compiles its shaders with
`glslc` or `glslangValidator` and embeds the result. That is a build-time
dependency on the user's side and nothing at run time, and it means the first
usable version needs no compiler work from us at all.

Writing GLSL → SPIR-V ourselves comes later, and removes that last external
tool. It is a real compiler — lexer, parser, a type system with vectors,
matrices and swizzles, and a SPIR-V emitter — somewhere around 6–10k lines for a
useful subset. One frontend, serving all three consumers.

Further out, shaders could be written in Caustic itself, which would make the
software path native code with no translation and would need the SPIR-V compiler
backend that was deferred. Not a near-term question.

**The software backend is where the cost actually lands.** Vulkan and OpenGL
receive SPIR-V and are done; the rasterizer has to *run* it. Three ways, and the
gap between them is large:

| | To write | Speed |
|---|---|---|
| Interpret SPIR-V | ~2–3k lines | a dispatch per instruction **per pixel** — a 50-instruction fragment shader at 640×480 is 15M interpreter steps a frame |
| Compile ahead of time, at build | less | native, but no dynamic shaders |
| Compile at run time | most | native |

Interpretation is where this starts, and it is honest to say that while it
interprets, the dependency-free path is a demonstration rather than a production
option.

Run-time compilation is worth noting only for what it would require: linking a
compiler into the renderer, which this library does not do and does not
currently plan to. SPIR-V being SSA, and `src/ir/ssa.cst` being SSA, means the
mapping would be natural if that ever changed — but it is a future architecture,
not a present one.

---

## Open questions

**Fallback needs runtime loading.** `AUTO` trying Vulkan and falling back to
OpenGL cannot use `DT_NEEDED`: a binary that declares `libvulkan.so.1` needed
will not start on a machine without it — the loader refuses before `main`. Real
fallback wants a `dlopen` of our own. `std/csl_loader.cst` is prior art: it
already mmaps images and resolves symbols.

Vulkan softens this considerably, since resolving one symbol is enough to reach
the whole API.

---

## Order of work

1. **`vk.xml` generator**, and Vulkan against a single window backend. Vulkan
   first because it proves the native-handle contract, and because its loader
   model means the binding is a dispatch table rather than 1719 externs.
   Shaders arrive as precompiled SPIR-V, so nothing here waits on a shader
   compiler.
2. **`gl.xml` generator** and OpenGL, which shares the machinery and takes the
   same SPIR-V.
3. **C header parser + COM extractor**, then Direct3D, verified under wine.
4. **GLSL → SPIR-V**, once the backends it feeds exist and have proven what the
   frontend actually has to emit.

Vulkan before a second window backend exists: it is the GPU path that tells us
whether the handle contract is right, and that is cheaper to learn on one
platform than on three.
