# Direct3D

The Windows backend. Vulkan also runs on Windows, so this is not strictly
required — it is here because on Windows it is the path with the best driver
support, and because "everything the platform exposes" was the premise.

```
d3d/
  d3d12.cst     generated — 62 interfaces
  dxgi.cst      generated — 14 interfaces: adapters, swapchain, presentation
  backend.cst   gpu/device.cst implemented over them
```

---

## The surface

| | Methods | Interfaces | Header |
|---|---|---|---|
| **D3D12** | 1572 | 62 | 1.2 MB |
| **D3D11** | 569 | 41 | 460 KB |
| **DXGI** | 170 | 14 | 95 KB |

Headers available locally through mingw (`/usr/x86_64-w64-mingw32/include/`),
so this is generated and tested from Linux.

**D3D12 rather than D3D11.** It maps onto the same explicit model as Vulkan —
command lists, pipeline state objects, descriptor heaps, explicit
synchronisation — so it shares the shape of `gpu/device.cst` instead of fighting
it. D3D11's implicit device context is the OpenGL problem again, and there is no
reason to take it on twice.

DXGI is shared: it owns adapter enumeration and the swapchain for both.

---

## It is COM, and that is the whole difficulty

Vulkan and OpenGL are flat C functions. Direct3D is interfaces with virtual
method tables:

```
object          -> [0]  pointer to vtable
vtable          -> [n]  pointer to the method
call(method, object, args...)          object is the implicit first argument
```

Caustic has `fn_ptr` and `call`, so the mechanism exists. What the binding must
carry is **the vtable slot of every method**, and a slot is determined by
declaration order within the interface, *including everything inherited*. Every
COM interface derives from `IUnknown`, so slots 0, 1 and 2 are always
`QueryInterface`, `AddRef` and `Release`, and a method's own index starts after
its entire ancestry.

Get a slot wrong and the call lands on a different method with a different
signature. There is no diagnostic — it is the same class of silent failure as a
wrong struct offset, with **2311 opportunities** across the three headers.

That is why the extractor is a tool and not a typing exercise, and why it is the
one generator with no counterpart anywhere else in the project: it must read
interface declarations, resolve inheritance chains, and emit slot numbers.

**Reference counting comes with COM.** Every interface is `AddRef`/`Release`, and
`gpu/device.cst` has to decide whether it owns those calls or exposes them. Since
the rest of the library uses explicit handles with explicit destruction, the
backend should own them and never let a refcount escape.

---

## The swapchain is DXGI's

`IDXGIFactory::CreateSwapChainForHwnd` takes the `HWND` from
`window.win32.hwnd`, which is the same native handle Vulkan's
`VK_KHR_win32_surface` takes. So the contract established in
[`window/window.md`](../../window/window.md) serves both without change — which
is a useful confirmation that it was drawn in the right place.

---

## Testing

Wine implements a substantial part of D3D11 and a growing part of D3D12, mapping
them onto Vulkan. That is enough to exercise the binding — that slots are right,
that calls reach the right method, that a swapchain gets created — and not enough
to say anything about real driver behaviour or performance.

So: wine proves the binding, and only a Windows machine proves the backend. The
same split the Win32 window backend has.

---

## Order of work

Last, after Vulkan has settled the shape of `gpu/device.cst` and the C header
parser exists.

1. **C header parser**, shared with X11, DRM and Win32.
2. **COM vtable extractor** — inheritance chains and slot numbers.
3. **DXGI**, which is small and needed by everything else here.
4. **D3D12 backend**, following whatever Vulkan settled.
