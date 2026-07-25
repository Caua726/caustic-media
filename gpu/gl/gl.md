# OpenGL

The compatibility backend. Slower to reach the hardware than Vulkan and far more
widely present — old drivers, virtual machines, remote sessions, anything where
Vulkan is missing or broken.

```
gl/
  opengl.cst    generated from gl.xml — commands and enums per version
  backend.cst   gpu/device.cst implemented over them
```

---

## The surface

| | |
|---|---|
| `gl*` exported by libGL | **3470** |
| Prototypes in `glext.h` | 5283 |
| `gl.xml` | the Khronos registry — **not installed here**, downloadable |
| OpenGL available on this machine | **4.6 core**, Mesa 26.1.5 |

Generating per version and per profile matters more here than in Vulkan: nobody
wants all 3470. A build targets a version — 3.3 core, 4.6 core — and gets the
commands and enums that version defines, plus whichever extensions were asked
for. `gl.xml` carries exactly that partitioning.

---

## It is a state machine, and our device is not

This is the impedance mismatch, and it is the interesting part of the backend.

`gpu/device.cst` says state travels with the work — a pipeline object carries
blend, depth, culling and shaders, and binding it sets all of them. OpenGL has no
such object: `glEnable(GL_BLEND)`, `glBlendFunc`, `glDepthFunc`, `glCullFace`,
`glUseProgram` are independent global switches.

So `bind_pipeline` in this backend expands into a sequence of state calls. Two
things follow:

**Redundant state changes have to be filtered.** Setting the same blend mode
every draw costs real time in the driver. The backend keeps a shadow copy of the
state it believes is set and skips calls that would not change anything — which
is exactly what the pipeline object gives you for free elsewhere.

**Some pipeline state has no OpenGL equivalent and must be emulated or refused.**
The capability query in `device.cst` exists for this: it is better to answer
honestly than to accept a pipeline and silently ignore half of it.

Vertex layouts map onto VAOs, which are themselves objects — the one place
OpenGL agrees with the model.

---

## Context creation is not OpenGL's job

There is no portable way to make a GL context; it belongs to the platform:

| Platform | API |
|---|---|
| X11 | GLX, or EGL |
| Wayland | **EGL only** — no GLX |
| KMS | EGL with `EGL_MESA_platform_gbm` |
| Windows | WGL |

EGL covers three of the four and is 65 KB, so it is the sane default on Linux,
with WGL bound alongside for Windows. Either way the context comes from the
window's native handles, the same ones Vulkan's surface comes from.

This is also where vsync lives — `eglSwapInterval` / `wglSwapIntervalEXT` — and
where `swapchain.cst` maps onto `eglSwapBuffers`, since OpenGL has no swapchain
object of its own.

---

## SPIR-V works here too

`GL_ARB_gl_spirv` has been core since **4.6**, through `glShaderBinary` +
`glSpecializeShader`. So the same precompiled SPIR-V the Vulkan backend consumes
goes straight into OpenGL, and there is no second shader path — which is what
made SPIR-V the internal currency in [`../gpu.md`](../gpu.md) rather than merely
Vulkan's input format.

Below 4.6 it needs GLSL, which means either shipping both or translating
SPIR-V back to GLSL. That is a real decision but not an urgent one: a machine old
enough to lack 4.6 is a machine where the software backend may be the better
answer anyway.

---

## Compute

Compute shaders are core since 4.3, so `gpu/`'s compute path has an OpenGL
implementation. Tessellation is core since 4.0, geometry shaders since 3.2. The
capability query reports what the actual context supports rather than what the
header declares.

---

## Order of work

After Vulkan, sharing the generator machinery.

1. **`gl.xml` generator**, emitting a chosen version and profile.
2. **EGL context creation** over the window's native handles.
3. **The state shadow**, without which every draw pays for state it already had.
4. **SPIR-V path** on 4.6, GLSL fallback decided only if something needs it.
