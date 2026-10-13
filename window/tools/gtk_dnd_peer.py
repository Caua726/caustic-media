#!/usr/bin/env python3
# An independent drag-and-drop peer for the window layer's tests: a GTK 3
# window at a known place on the screen, as GTK itself speaks XDND (X11) or
# wl_data_device (Wayland). It writes what happens, a line each, to the file
# named by its second argument:
#
#   gtk_dnd_peer.py target <log> [x y]
#       takes text/uri-list and text by copy or move; logs "ready", then
#       "drop <target> <action> <bytes>" per drop, its bytes written to
#       <log>.<n>, n counting the drops from 1
#   gtk_dnd_peer.py source <log> files|text|bigtext [x y]
#       drags what it is told from a press inside it; logs "ready", then
#       "get <target>" per request and "end <action>" when the drag ends
#
# The window is 300 by 300 at x, y (600, 100 when not said), undecorated.
import sys

import gi

gi.require_version("Gtk", "3.0")
gi.require_version("Gdk", "3.0")
from gi.repository import Gdk, GLib, Gtk  # noqa: E402

mode = sys.argv[1]
log = open(sys.argv[2], "w", buffering=1)
args = sys.argv[3:]
what = None
if mode == "source":
    what = args.pop(0)
x = int(args[0]) if len(args) > 0 else 600
y = int(args[1]) if len(args) > 1 else 100


def say(line):
    log.write(line + "\n")
    log.flush()


def action_name(a):
    if a & Gdk.DragAction.MOVE:
        return "move"
    if a & Gdk.DragAction.COPY:
        return "copy"
    return "none"


win = Gtk.Window()
win.set_decorated(False)
win.set_title("dnd-peer")
win.set_default_size(300, 300)
win.move(x, y)
box = Gtk.EventBox()
box.set_size_request(300, 300)
win.add(box)

targets = [
    Gtk.TargetEntry.new("text/uri-list", 0, 1),
    Gtk.TargetEntry.new("UTF8_STRING", 0, 2),
    Gtk.TargetEntry.new("text/plain;charset=utf-8", 0, 3),
]
actions = Gdk.DragAction.COPY | Gdk.DragAction.MOVE

if mode == "target":
    box.drag_dest_set(Gtk.DestDefaults.ALL, targets, actions)
    drops = [0]

    def received(widget, context, px, py, data, info, time):
        raw = data.get_data() or b""
        drops[0] += 1
        with open("%s.%d" % (sys.argv[2], drops[0]), "wb") as f:
            f.write(raw)
        say("drop %s %s %d" % (data.get_target().name(), action_name(context.get_selected_action()), len(raw)))

    box.connect("drag-data-received", received)
else:
    src_targets = targets[:1] if what == "files" else targets[1:]
    box.drag_source_set(Gdk.ModifierType.BUTTON1_MASK, src_targets, actions)

    def payload():
        if what == "files":
            return b"file:///tmp/peer%20one.txt\r\nfile:///tmp/peer-two\r\n"
        if what == "bigtext":
            line = "gtk ação — through INCR ✓ ".encode()
            return (line * (3 * 1024 * 1024 // len(line) + 1))[: 3 * 1024 * 1024]
        return "from gtk: olá ✓".encode()

    def get(widget, context, data, info, time):
        name = data.get_target().name()
        say("get %s" % name)
        raw = payload()
        if what == "files" and name != "text/uri-list":
            return
        if what != "files" and name == "text/uri-list":
            return
        data.set(data.get_target(), 8, raw)

    def end(widget, context):
        say("end %s" % action_name(context.get_selected_action()))

    box.connect("drag-data-get", get)
    box.connect("drag-end", end)

    def failed(widget, context, result):
        say("failed %d" % int(result))
        return False

    box.connect("drag-failed", failed)

win.connect("destroy", Gtk.main_quit)


def mapped(*_):
    GLib.timeout_add(200, lambda: (say("ready"), False)[1])


win.connect("map-event", mapped)
win.show_all()
Gtk.main()
