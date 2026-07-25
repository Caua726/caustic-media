# Win32

The Windows backend.

```
win32/
  user32.cst    windows, messages, input
  gdi32.cst     device contexts, DIB sections, blitting
  backend.cst   window/device.cst implemented over them
```

Caustic already cross-compiles to PE and already declares DLL imports —
`std/os/windows.cst` binds kernel32, ws2_32 and bcrypt — so the mechanism is
proven. This is the same mechanism at a larger scale.

---

## The surface

| | |
|---|---|
| user32 + gdi32 exports | ≈ **1600** |
| The windowing core | ~150 |
| `winuser.h` | 224 KB |
| `wingdi.h` | 136 KB |

Headers are available locally through mingw (`/usr/x86_64-w64-mingw32/include/`)
and wine, so bindings can be generated and the result tested on Linux without
leaving the machine.

The core, all confirmed present in those headers:

```
RegisterClassExA  CreateWindowExA  ShowWindow  DestroyWindow  AdjustWindowRect
GetMessageA  PeekMessageA  TranslateMessage  DispatchMessageA  DefWindowProcA
GetClientRect  SetWindowTextA
CreateDIBSection  StretchDIBits  SetDIBitsToDevice  BitBlt  CreateCompatibleDC
```

---

## The message pump is a callback, and that matters

Win32 does not hand you events to read. You register a **window procedure** and
the system calls it:

```
RegisterClassExA(&wc)      // wc.lpfnWndProc = our function
...
while (GetMessageA(&msg, ...)) {
    TranslateMessage(&msg);
    DispatchMessageA(&msg);   // -> calls our WndProc, possibly reentrantly
}
```

Two consequences for this backend:

**A function pointer crosses back into Caustic.** `WndProc` is called *by
Windows*, with the MS x64 calling convention, possibly during
`DispatchMessageA`, possibly during `CreateWindowExA` before it has even
returned. Caustic has `fn_ptr`, so this works, but it is the first place in the
library where foreign code calls into ours rather than the reverse, and the
callback must not assume the window struct is fully initialised.

**Modal loops steal the thread.** Dragging or resizing a window puts Windows into
its own message loop inside `DispatchMessageA`, which does not return until the
user lets go. A program that renders only in its own loop freezes while being
resized — the standard fix is to render from a `WM_TIMER` or `WM_PAINT` during
those, which is worth building in rather than discovering.

So the portable `next_frame` here drains the pump and returns; Win32 imposes no
pacing of its own, like X11.

---

## Presenting a software frame

`CreateDIBSection` gives a bitmap whose pixels are directly addressable memory —
so the same arrangement as everywhere else: the backend owns the buffer, hands it
out per frame, and `StretchDIBits` or `BitBlt` carries it to the window.

Channel order is `0xAARRGGBB` on little-endian, which is what
`math/color.pack_argb8` already produces — so unlike X11 there is no swizzle in
the copy.

---

## Native handles

```cst
window.win32.hwnd(&w)        // HWND       -> VK_KHR_win32_surface, WGL
window.win32.hinstance(&w)   // HINSTANCE
```

---

## Testing without Windows

Wine runs the result, and this is already how the Caustic toolchain's own Windows
builds are verified — the PE `hello.exe` in the compiler's test tree runs under
wine on this machine.

What wine does *not* prove: driver behaviour, DPI scaling across real monitors,
and anything about D3D beyond what wine implements. Enough for the backend to be
correct; not enough to call it tested on Windows.

---

## Order of work

Last of the four, per [`../window.md`](../window.md) — after the C header
generator exists, since 1600 exports is not something to type.

1. **Generate `user32.cst` and `gdi32.cst`** from the mingw headers.
2. **Window class, creation, the pump**, with `WndProc` as a Caustic `fn_ptr`.
3. **DIB section presentation.**
4. **Modal-loop rendering**, so resizing does not freeze the picture.
