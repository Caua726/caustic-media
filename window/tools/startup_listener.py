#!/usr/bin/env python3
# An independent startup-notification listener for the window layer's X11
# tests, written with python-xlib (none of this repository's code): what a
# launcher's taskbar does — _NET_STARTUP_INFO_BEGIN and _NET_STARTUP_INFO
# client messages on the root, twenty bytes each, put back together per
# sending window until the NUL that ends a message — and what a window
# manager hears asked of it: _NET_ACTIVE_WINDOW, its source and timestamp.
#
#   startup_listener.py <seconds> [removes]
#
# Prints "listening" once it watches, then each startup message whole and
# each activation request as "active <source> <timestamp>", a line each,
# flushed; quits after the given number of "remove:" messages (one unless
# said) or when the time is up.
import sys
import time

from Xlib import X, display

d = display.Display()
root = d.screen().root
root.change_attributes(event_mask=X.PropertyChangeMask | X.SubstructureNotifyMask)
begin = d.intern_atom("_NET_STARTUP_INFO_BEGIN")
more = d.intern_atom("_NET_STARTUP_INFO")
active = d.intern_atom("_NET_ACTIVE_WINDOW")
d.sync()
print("listening", flush=True)
pending = {}
removes = int(sys.argv[2]) if len(sys.argv) > 2 else 1
end = time.time() + float(sys.argv[1])
while time.time() < end:
    if d.pending_events() == 0:
        time.sleep(0.01)
        continue
    e = d.next_event()
    if e.type != X.ClientMessage:
        continue
    if e.client_type == active:
        words = e.data[1]
        print("active %d %d" % (words[0], words[1]), flush=True)
        continue
    if e.client_type not in (begin, more):
        continue
    data = bytes(e.data[1])
    if e.client_type == begin:
        pending[e.window.id] = b""
    if e.window.id not in pending:
        continue
    pending[e.window.id] += data
    if b"\0" in pending[e.window.id]:
        text = pending.pop(e.window.id).split(b"\0")[0].decode("utf-8", "replace")
        print(text, flush=True)
        if text.startswith("remove:"):
            removes -= 1
            if removes == 0:
                break
