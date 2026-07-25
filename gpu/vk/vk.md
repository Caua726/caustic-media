# Vulkan

The backend that goes first, for two reasons: it is what proves the
native-handle contract with [`window/`](../../window/window.md), and its loader
model means the binding is a dispatch table rather than seventeen hundred extern
declarations.

```
vk/
  vulkan.cst    generated from vk.xml — commands, structs, enums, handles
  backend.cst   gpu/device.cst implemented over them
```

---

## The surface

| | |
|---|---|
| Commands | **1719** |
| Structs | **1606** |
| Enum groups | 330 |
| Handle types | 61 |
| Extensions | 665 |
| `vk.xml` | 3 MB, present at `/usr/share/vulkan/registry/` |

`vulkan_core.h` is 1.2 MB of *generated* output. Nobody types these, including
Khronos.

---

## Almost none of it is linked

`libvulkan.so.1` exports **269** of those 1719 commands. Everything else — and
every extension, and the swapchain is an extension — is fetched at run time:

```
vkGetInstanceProcAddr(instance, "vkCreateSwapchainKHR")
vkGetDeviceProcAddr(device, "vkQueueSubmit")
```

So the binding is **one extern plus a dispatch table**, filled at run time and
called through `fn_ptr`/`call`. Three consequences:

**A binary can carry Vulkan with no `DT_NEEDED`.** Resolve `vkGetInstanceProcAddr`
by name — through our own loader, since `std/csl_loader.cst` already mmaps images
and resolves symbols — and derive everything else. That is the difference between
a program that refuses to start without Vulkan installed and one that tries
Vulkan and falls back. It is also what makes `AUTO` possible at all.

**Extensions cost nothing structurally.** An extension command is another entry
in the same table.

**The table has three levels, and conflating them costs performance on every
call:**

```
loader     vkGetInstanceProcAddr, vkCreateInstance, vkEnumerateInstanceExtensionProperties
instance   vkEnumeratePhysicalDevices, vkCreateDevice, surface and display commands
device     vkQueueSubmit, vkCmdDraw, everything in the hot path
```

A device-level command fetched through `vkGetInstanceProcAddr` still works, but
it goes through a loader trampoline that dispatches on the handle — a lookup on
every single call. Fetched through `vkGetDeviceProcAddr` it is direct.
**`vk.xml` says which is which**, so the generator can split the table correctly
without anyone having to remember.

---

## What the generator has to emit

- **Commands**, tagged instance-level or device-level, into two dispatch tables.
- **Structs**, with `sType` values — Vulkan's extension mechanism is a linked
  list through `pNext`, so every struct starts with a tag and a pointer.
- **Enums**, including the extension-number arithmetic that assigns values to
  extension enumerants.
- **Handles**, as opaque 64-bit values. Note that non-dispatchable handles are
  `u64` even on 32-bit, which is a real portability trap.
- **Feature and extension membership**, so a build can generate core-only or
  core-plus-a-chosen-set rather than all 665.

The `pNext` chain deserves attention: it is how every modern feature is enabled,
it is untyped in C, and getting it wrong is silent. Whatever shape it takes in
Caustic should make the tag and the struct impossible to mismatch.

---

## The surface comes from the window

```
VK_KHR_xlib_surface      Display* + Window        window.x11.display / .xid
VK_KHR_wayland_surface   wl_display* + wl_surface window.wayland.display / .surface
VK_KHR_win32_surface     HINSTANCE + HWND         window.win32.hinstance / .hwnd
```

KMS has no surface: there is no compositor to present to. It uses
`VK_KHR_display` and drives the connector directly, which is the same shape as
the KMS backend's own scanout path.

Once a surface exists, **the swapchain owns presentation** and the window's
software blit path is bypassed entirely. That is the reason
[`window/window.md`](../../window/window.md) makes native handles public
contract.

---

## The parts that are genuinely hard

Vulkan is explicit about things other APIs hide, and a device abstraction has to
decide what to hide again.

**Memory.** Vulkan gives heaps and types and expects the application to suballocate;
allocating one `VkDeviceMemory` per buffer exhausts the driver's allocation limit
quickly. A real backend needs a suballocator — which is what VMA is for C++, and
which we would write.

**Descriptor sets.** Binding resources means pools, layouts and sets, with
lifetime rules. This is where most of a Vulkan backend's complexity ends up, and
where the abstraction earns its keep.

**Synchronisation.** Semaphores, fences, pipeline barriers, image layout
transitions. Everything is explicit and nothing is checked at run time unless the
validation layers are enabled — so the backend should support enabling them, and
should say so in `last_error`.

**Frames in flight.** The CPU runs ahead of the GPU, so command buffers,
descriptor sets and uniform buffers are per-frame and cycled. This interacts
directly with the window's per-frame buffer contract, and getting it wrong shows
up as flickering rather than as an error.

---

## Order of work

1. **The `vk.xml` generator** — the dispatch tables, structs and enums.
2. **Instance, physical device selection, logical device and queues.**
3. **Surface and swapchain** from the window's native handles.
4. **Memory suballocator, descriptor management, frames in flight.**
5. **Compute**, which shares everything above and adds only a pipeline type.

Vulkan lands against **one** window backend before a second exists — the handle
contract is cheaper to learn on one platform than on three.
