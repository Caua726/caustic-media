# ui/atspi/tools/atspi_check.py — libatspi reads atspi_peer through the
# desktop's own accessibility bus and registry. Run by atspi_check.sh.
import sys
import time

import gi

gi.require_version("Atspi", "2.0")
from gi.repository import Atspi, GLib  # noqa: E402

fails = 0


def check(what, ok):
    global fails
    if not ok:
        print("FAIL: " + what)
        fails += 1


def spin(seconds):
    ctx = GLib.MainContext.default()
    end = time.time() + seconds
    while time.time() < end:
        while ctx.iteration(False):
            pass
        time.sleep(0.01)


def find_app(name, seconds):
    end = time.time() + seconds
    while time.time() < end:
        desktop = Atspi.get_desktop(0)
        for i in range(desktop.get_child_count()):
            a = desktop.get_child_at_index(i)
            if a is not None and a.get_name() == name:
                return a
        time.sleep(0.1)
    return None


def child(parent, name):
    for i in range(parent.get_child_count()):
        c = parent.get_child_at_index(i)
        if c is not None and c.get_name() == name:
            return c
    return None


def child_of_role(parent, role):
    for i in range(parent.get_child_count()):
        c = parent.get_child_at_index(i)
        if c is not None and c.get_role() == role:
            return c
    return None


def has(obj, state):
    return obj.get_state_set().contains(state)


def action(obj, name):
    for i in range(Atspi.Action.get_n_actions(obj)):
        if Atspi.Action.get_action_name(obj, i) == name:
            return i
    return -1


heard = []


def on_event(e):
    heard.append((e.type, e.source.get_name() if e.source else None, e.detail1))


def main():
    Atspi.init()
    listener = Atspi.EventListener.new(on_event)
    listener.register("object:state-changed:checked")
    listener.register("object:state-changed:focused")
    listener.register("object:text-changed:insert")

    app = find_app("caustic-peer", 8.0)
    check("the application, listed by the registry", app is not None)
    if app is None:
        return
    check("its toolkit", app.get_toolkit_name() == "caustic")
    check("an application", app.get_role() == Atspi.Role.APPLICATION)
    win = app.get_child_at_index(0)
    check("its window, a frame named by its title",
          win is not None and win.get_role() == Atspi.Role.FRAME and win.get_name() == "Peer window")
    check("the window's parent: the application", win.get_parent().get_name() == "caustic-peer")

    ok = child(win, "OK")
    check("a button", ok is not None and ok.get_role() == Atspi.Role.BUTTON)
    check("enabled, showing, focusable",
          has(ok, Atspi.StateType.ENABLED) and has(ok, Atspi.StateType.SHOWING) and
          has(ok, Atspi.StateType.VISIBLE) and has(ok, Atspi.StateType.FOCUSABLE))
    check("its index", ok.get_index_in_parent() == 0)
    said = child_of_role(win, Atspi.Role.LABEL)
    check("a label", said is not None and Atspi.Text.get_text(said, 0, -1) == "ready")
    i = action(ok, "click")
    check("clicked, by its action", i >= 0 and Atspi.Action.do_action(ok, i))
    spin(0.3)
    check("and it was", Atspi.Text.get_text(said, 0, -1) == "clicked 1")
    check("the label's name follows", said.get_name() == "clicked 1")

    box = child(win, "Remember")
    check("a check box", box is not None and box.get_role() == Atspi.Role.CHECK_BOX and
          not has(box, Atspi.StateType.CHECKED))
    heard.clear()
    Atspi.Action.do_action(box, action(box, "toggle"))
    spin(0.5)
    check("toggled", has(box, Atspi.StateType.CHECKED))
    check("and said so", ("object:state-changed:checked", "Remember", 1) in heard)

    slider = child_of_role(win, Atspi.Role.SLIDER)
    check("a slider, its range",
          slider is not None and Atspi.Value.get_minimum_value(slider) == 0.0 and Atspi.Value.get_maximum_value(slider) == 10.0)
    check("its value set", Atspi.Value.set_current_value(slider, 7.0) and Atspi.Value.get_current_value(slider) == 7.0)

    e = child_of_role(win, Atspi.Role.ENTRY)
    check("an entry, its text", e is not None and Atspi.Text.get_text(e, 0, -1) == "hello" and Atspi.Text.get_character_count(e) == 5)
    check("editable, one line", has(e, Atspi.StateType.EDITABLE) and has(e, Atspi.StateType.SINGLE_LINE))
    heard.clear()
    check("text inserted", Atspi.EditableText.insert_text(e, 5, " world", 6))
    spin(0.5)
    check("there", Atspi.Text.get_text(e, 0, -1) == "hello world")
    check("and said so", any(h[0] == "object:text-changed:insert" for h in heard))
    word = Atspi.Text.get_text_at_offset(e, 7, Atspi.TextBoundaryType.WORD_START)
    check("a word", word.content == "world" and word.start_offset == 6 and word.end_offset == 11)
    check("the caret put", Atspi.Text.set_caret_offset(e, 2) and Atspi.Text.get_caret_offset(e) == 2)

    ext = Atspi.Component.get_extents(ok, Atspi.CoordType.WINDOW)
    check("where the button is", ext.width > 0 and ext.height > 0)
    at = Atspi.Component.get_accessible_at_point(win, ext.x + 2, ext.y + 2, Atspi.CoordType.WINDOW)
    check("what is at a point", at is not None and at.get_name() == "OK")
    heard.clear()
    check("the focus asked for", Atspi.Component.grab_focus(e))
    spin(0.5)
    check("focused", has(e, Atspi.StateType.FOCUSED))
    check("and said so", any(h[0] == "object:state-changed:focused" and h[2] == 1 for h in heard))

    quit_button = child(win, "Quit")
    check("quit asked for", quit_button is not None and Atspi.Action.do_action(quit_button, action(quit_button, "click")))


try:
    main()
except Exception as x:  # a failure of the protocol shows up as an exception from libatspi
    print("FAIL: " + repr(x))
    fails += 1
if fails:
    print("atspi libatspi: %d failures" % fails)
    sys.exit(1)
print("atspi libatspi: all checks passed")
