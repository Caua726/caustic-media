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

A window seen through (`Config.transparent`, only where the system draws no
frame round it) is a layered window (`WS_EX_LAYERED`): each frame is given to
the system with `UpdateLayeredWindow` from a second DIB, the frame's
premultiplied RGBA as the premultiplied BGRA it blends — the whole window,
since such a window has no frame of the system's. Framed by the system later
(`set_decorated(1)`), it stops being layered.

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

Wine's X11 driver on an X server with no compositing manager shows a layered
window's alpha as all or nothing (0 shaped out, the rest unblended), so the
blend is proven with its Wayland driver instead: `tools/wine_session.sh` with
`CAUSTIC_WINE_GRAPHICS=wayland` runs the session's Wine against a Weston
nested in its Xvfb (`WESTON_CONFIG` passed on), and the X screen still reads
back what that compositor composed.

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
It handles close requests, resize, UTF-16 character input, IMM32 composition —
an input context only while a text field that is not a password's has the
keyboard, the target clause as the part converted, candidates excluded from
the caret, the field's text for `IMR_DOCUMENTFEED` — pointer capture and wheel
deltas — the input translated into
`input/event.cst`'s cooked events as it arrives: a key's scan code as its
place and bit 30 as the system's repeat, a `WM_CHAR` the key's only right
after it went down, focus loss a `CANCEL` that lets the buttons and the
capture go, a press a system move takes ending in `BUTTON_TAKEN` and a
release of a button not held unsaid — plus modal owners,
fullscreen restore, size constraints and native cursors. Unicode clipboard
ownership and reads use `CF_UNICODETEXT`. `settings.cst` reads the desktop's
locale, font, DPI, timings, high contrast, animation preference and available
registry/DWM preferences.

`backend_test.cst` exercises actual windows under Wine/Xvfb: presented pixels,
keys with their place and the system's repeat, Unicode surrogate pairs, whose
a character is, control-character filtering, a full input queue keeping what
came first and counting what it refused, pointer capture, a release of a
button not held, focus loss letting everything go, half and whole wheel
notches, modal ownership, clipboard transfer,
resize, fullscreen restore and a close request that can be declined. Desktop
preference reads were also exercised in a native smoke. Real Windows, monitor
transitions and interaction with an installed IME remain unverified.

`../keyboard_win32_test.cst` sends keys as a keyboard does (SendInput): the
key a virtual key names and its scan code, a key down again while down as the
system's repeat, Caps Lock, Alt as `WM_SYSKEYDOWN`, Control's letter as a
command with no text, and a Russian layout loaded and in use
(`LoadKeyboardLayoutW`, `ActivateKeyboardLayout`): the key and its shortcut
stay the virtual key's Latin letter, so Ctrl+C is Ctrl+C. Wine translates
SendInput's keys to characters through its X keymap whatever layout is
loaded, so the text a Russian or German layout (AltGr's included) types is
left to real Windows.

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
show/hide, client position, key/text/pointer/wheel input, the motion of a
frame as `input/state.cst` adds it up, the synchronous clipboard (no PRIMARY), floor and
ceiling sizes, maximize/restore/fullscreen/minimize state events, activation
from behind another window, text input and the IME candidate position, the
desktop's settings, icons (size bounds, rows top-down as read back), the system
cursor each portable shape shows, decorations, frame extents, an input region
exact to its edges (HTTRANSPARENT outside), the command each move or resize
edge gives the system and the press it takes, a system move from a real synthetic left-button drag,
the system menu's modal loop until Escape and with Close chosen, an owned modal
dialog and close requests. It found that `WM_NCCREATE` skipped the default
procedure, so creation titles were lost, and that `resized`, the motion and the
wheel were never per frame (a window looked resized every frame after its
first); both are fixed.

A window drawing its own frame answers hit-testing for its edges: between the
window as the desktop counts it (inside the frame extents) and the input
region's edge — its edges' reach beyond it — `WM_NCHITTEST` says `HTLEFT` …
`HTBOTTOMRIGHT`, a corner taking twice that reach along each side as the
toolkit's frame does, and the system's own sizing loop follows the press; not
for a framed, fixed-size, maximized or full-screen window or a popup. A dialog
drawing its own frame is no longer given the system's caption. A move to a
monitor of another DPI (`WM_DPICHANGED`) takes the size the system suggests
and says `RESIZE` then `SETTINGS`, so the toolkit lays out again at the new
scale. Minimized is `STATE_MINIMIZED`, the window's size kept while it is
not shown; `WM_DWMCOMPOSITIONCHANGED` is `COMPOSITOR`. `monitor_of` says the
monitor it is most on: its rectangle and work area, the DPI there
(`GetDpiForMonitor`), its refresh rate (`EnumDisplaySettingsW`, 0 where the
system knows none, as Wine on Xvfb) and device name. `device_win32_test`
checks the edges' answers and a real resize from the right edge by the
system's loop (SendInput), a synthetic `WM_DPICHANGED` and back, minimized
and restored, the monitor against `GetMonitorInfoW`, and a layered window:
its opaque pixels exact on the screen, not layered once framed or when asked
for with a frame. `../transparent_win32_test.cst` with `tools/alpha_check.cst`
(`tools/run_alpha.sh`) shows a half-seen red blended over the blue desktop of
the nested Weston as 0x80007f and the blue itself through the clear part.
`../../ui/program_win32_test.cst` checks the toolkit following the DPI change:
laid out and painted at the new scale — the host had kept the desktop's
system DPI over the window's own, which only X11's screen-wide DPI should do.

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
