# The GPU layer

Same shape as [`window/`](../window/window.md): every function each graphics API
exposes, declared in Caustic, with the portable abstraction built on top of those
bindings rather than on FFI directly. A program either takes our device API or
reaches straight for `gpu.vk.*` and drives Vulkan itself.

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

## What the abstraction on top looks like

Fixed by the [design already agreed](../README.md): `gpu/` is device access —
buffers, pipelines, submission — and stops there. `render/gpu/` is the opinion
built on it. A program that wants to drive Vulkan itself takes `gpu/` and leaves
the rest.

```cst
// our way
let is media.gpu.Device as d = media.gpu.open(&win, media.gpu.AUTO);

// name the backend
let is media.gpu.Device as d = media.gpu.open(&win, media.gpu.VULKAN);

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

---

## Open questions

**Shaders are unsolved and they decide the shape of everything above `gpu/`.**
Vulkan consumes SPIR-V, OpenGL consumes GLSL, and the software rasterizer
consumes neither. One source has to reach all three. Three ways out, in
ascending ambition:

1. Accept GLSL and translate — SPIR-V for Vulkan, pass-through for GL, interpret
   for software.
2. Define our own shading language, one frontend and three backends.
3. **Write shaders in Caustic.** For the software backend that is native code
   with no translation at all; for Vulkan it needs the SPIR-V compiler backend
   that was deferred. Most on-brand and most work.

Nothing in `gpu/` needs the answer, but `render/gpu/` cannot be written without
it.

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
2. **`gl.xml` generator** and OpenGL, which shares the machinery.
3. **C header parser + COM extractor**, then Direct3D, verified under wine.

Vulkan before a second window backend exists: it is the GPU path that tells us
whether the handle contract is right, and that is cheaper to learn on one
platform than on three.
