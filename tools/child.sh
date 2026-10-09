# tools/child.sh — sourced by the isolated runners (run_headless.sh,
# run_wayland.sh, wine_session.sh).
#
# run_child runs a command as the runner's child and waits for it. A shell
# defers a trapped signal until its foreground command ends, so a runner that
# ran its program in the foreground kept a hung program — and its server or
# prefix — alive after being told to stop. Here the program runs in the
# background, with the runner's standard input, in a process group of its own
# (setsid). A HUP, INT or TERM sent to the runner ends that whole group with
# TERM — an asynchronous child ignores SIGINT, so passing INT on would not —
# and the runner exits 128+signal; its EXIT trap then removes what it owns.
_child=""
_forward() {
    if [ -n "$_child" ]; then
        kill -TERM -- "-$_child" 2>/dev/null || kill -TERM "$_child" 2>/dev/null || true
    fi
    exit "$1"
}
trap '_forward 129' HUP
trap '_forward 130' INT
trap '_forward 143' TERM

run_child() {
    exec 3<&0
    if command -v setsid >/dev/null 2>&1; then
        setsid "$@" <&3 3<&- &
    else
        "$@" <&3 3<&- &
    fi
    _child=$!
    exec 3<&-
    _status=0
    wait "$_child" || _status=$?
    _child=""
    return "$_status"
}
