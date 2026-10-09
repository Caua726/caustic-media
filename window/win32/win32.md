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
theme, DPI, shell, OLE, UI Automation and registry APIs, records, GUIDs and
inherited COM vtable slots. Constants are explicit names plus prefix families
(`WM_`, `VK_`, `UIA_`, pointer/gesture flags, UIA enums …): every integer
enumerator or object-like macro with the prefix that is still defined after
the headers. `tools/layout.c` independently checks the exposed layouts and all
constant values with mingw-gcc; `layout_test.cst` checks Caustic's same sizes
and offsets. `tools/exports.c` loads every bound DLL and resolves every bound
function at run time, because one missing export stops an image from loading.
Anonymous unions and structs are told apart by where they are declared, not by
clang's USR alone: it gives every anonymous member of a record the same one, and
`DEVMODEW`'s two unions (16 and 4 bytes) came out as one until `layout_test`
caught the size.

Caustic functions retain their SysV convention in a PE. `abi.cst` bridges
indirect native calls and callbacks, including stack arguments and doubles;
callbacks preserve the full Win64 nonvolatile XMM registers. Callback sets are
writable during construction and read/execute only after `seal`; they must
outlive every foreign reference. `caustic-mk run test-win32-native` runs the
independent C layout/constant, export and ABI oracles, a real window procedure,
the native backend and the shell's real file-dialog COM object. These passed under
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

The compiler resolves the real `__caustic_chkstk` definition before PE
validation/relocation (8192-byte recursive locals pass at `-O0/-O1/-O2` and
through separate object linking), and the installed toolchain was validated
with `test-win32`, including the `win_image` PNG decoder. Calls into a library
the target cannot load are refused in whole-program builds and trapped without
an import where a whole module is emitted, so importing this backend never
adds a Win32 DLL to a Linux binary.

Portable dispatch is connected: `../device.cst` opens `win32` under `AUTO` on
Windows and `device.available(WIN32)` is 1. `device_win32_test.cst` drives the
portable API in an isolated Wine, at 96 and at 144 dpi (`CAUSTIC_WINE_DPI`):
per-monitor DPI awareness and the scale it gives, the class name as the
process's AppUserModelID, presented pixels, UTF-8 titles past the BMP,
show/hide, client position, key/text/pointer/wheel events, the resize, motion
and wheel as per-frame edges, the synchronous clipboard (no PRIMARY), floor and
ceiling sizes, maximize/restore/fullscreen/minimize state events, activation
from behind another window, text input and the IME candidate position, the
desktop's settings, icons (size bounds, rows top-down as read back), the system
cursor each portable shape shows, decorations, frame extents, an input region
exact to its edges (HTTRANSPARENT outside), the command each move or resize
edge gives the system, a system move from a real synthetic left-button drag,
the system menu's modal loop until Escape and with Close chosen, an owned modal
dialog and close requests. It found that `WM_NCCREATE` skipped the default
procedure, so creation titles were lost, and that `resized`, the motion and the
wheel were never per frame (a window looked resized every frame after its
first); both are fixed.

`../wait.cst` sleeps in `MsgWaitForMultipleObjectsEx` on Windows: the thread's
queue (input already looked at included), an auto-reset event for `wake`, and
watched HANDLEs (`wait_win32_test.cst`). A toolkit program starts and runs on
Windows (`../../ui/program_win32_test.cst`, at 96 and 144 dpi): its fonts
found and mapped (`text/fonts`), its window drawn — the painted text read back
from the window's DC — clicked and typed into, and closed by the system's
message. The first window is sized once its scale is known. Popups are
`WS_POPUP` tool windows never activated, placed within the monitor's work
area, capturing the pointer and taking the owner's keys, taken down by a press
outside the program's popups, by the capture going elsewhere and by the
application's deactivation (`../popup_win32_test.cst`, SendInput). The
toolkit's own menus are not yet hosted in them; OLE DnD, toolkit UIA, platform
choosers, notifications, complete CSD and modal-loop toolkit rendering remain
implementation work, and Wine is not proof of real-Windows behavior.
