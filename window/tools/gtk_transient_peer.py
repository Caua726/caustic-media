#!/usr/bin/env python3
# An independent xdg-foreign importer for the window layer's tests: a GTK 4
# window on Wayland made the child of another program's window by the handle
# that program exported (GdkWaylandToplevel.set_transient_for_exported,
# zxdg_importer_v2 underneath), as a portal's dialog is.
#
#   gtk_transient_peer.py <handle> <title>
#
# Prints "imported 1" (or 0 when GTK refused the handle) once its window is
# shown, flushed, and runs until it is stopped.
import sys

import gi

gi.require_version("Gtk", "4.0")
gi.require_version("GdkWayland", "4.0")
from gi.repository import GdkWayland, GLib, Gtk  # noqa: E402

handle = sys.argv[1]
title = sys.argv[2]
loop = GLib.MainLoop()
win = Gtk.Window(title=title)
win.set_default_size(200, 120)


def mapped(_widget):
    surface = win.get_surface()
    ok = isinstance(surface, GdkWayland.WaylandToplevel) and surface.set_transient_for_exported(handle)
    print("imported %d" % (1 if ok else 0), flush=True)


win.connect("map", mapped)
win.connect("close-request", lambda *_: loop.quit())
win.present()
loop.run()
