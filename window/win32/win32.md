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

The top-down DIB uses `BI_BITFIELDS` with RGBA byte masks matching the
software target. `BitBlt` presents that memory without a channel-swapping
copy. The native smoke checks the presented RGB values with GDI `GetPixel`.

---

## Native handles

```cst
use "window/win32/backend.cst" as win32;
win32.hwnd(&w)        // HWND
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

## Current state

The SDK boundary is generated in `bind/` by `tools/generate.py` (clang Python
bindings, mingw headers), selected by `tools/sdk.json`. Regenerate with
`python3 window/win32/tools/generate.py`. It covers the window, GDI, IMM32,
shell, OLE, UI Automation and registry APIs, records, constants, GUIDs and
inherited COM vtable slots. `tools/layout.c` independently checks the exposed
layouts with mingw-gcc; `layout_test.cst` checks Caustic's same sizes and offsets.

Caustic functions retain their SysV convention in a PE. `abi.cst` bridges
indirect native calls and callbacks, including stack arguments and doubles;
callbacks preserve the full Win64 nonvolatile XMM registers. Callback sets are
writable during construction and read/execute only after `seal`; they must
outlive every foreign reference. `caustic-mk run test-win32-native` runs the
independent C layout and ABI oracles, a real window procedure, the native
backend and the shell's real file-dialog COM object. These checks passed under
Wine, not on a real Windows installation. The group owns one private Wine
prefix and X server; it cannot reuse another test's stale native desktop.

`backend.cst` opens native windows with DPI-aware client geometry, a reentrant
procedure, a nonblocking message pump and directly writable software frames.
It handles close requests, resize, key state, UTF-16 character input, IMM32
composition messages, pointer capture, accumulated wheel notches, modal owners,
fullscreen restore, size constraints and native cursors. Unicode clipboard
ownership and reads use `CF_UNICODETEXT`. `settings.cst` reads the desktop's
locale, font, DPI, timings, high contrast, animation preference and available
registry/DWM preferences.

`backend_test.cst` exercises actual windows under Wine/Xvfb: presented pixels,
Unicode surrogate pairs, control-character filtering, queue overflow ordering,
pointer capture, partial wheel notches, modal ownership, clipboard transfer,
resize, fullscreen restore and a close request that can be declined. Desktop
preference reads were also exercised in a native smoke. Real Windows, monitor
transitions and interaction with an installed IME remain unverified.

The portable `device.cst` and toolkit Windows integration are blocked by the
Caustic linker: a program with an 8192-byte local array, using only `std/io`
and `std/mem`, fails with `undefined symbol: __caustic_chkstk`. The same blocker
also stops the existing `win_image` target before the later `test-win32`
checks. Adding this backend to the portable dispatcher additionally leaked
Windows DLL dependencies into Linux ELF examples despite OS guards; that
experimental dispatcher integration was removed, rather than shipping
unloadable Linux executables. The native backend is usable directly;
`device.available(WIN32)` remains false. OLE DnD, toolkit UIA, platform choosers,
complete CSD and modal-loop toolkit rendering are not implemented here.
