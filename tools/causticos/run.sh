#!/bin/bash
# tools/causticos/run.sh — caustic-media's tests run inside CausticOS.
#
#   tools/causticos/run.sh math/math_test.cst gpu/gpu_test.cst ...
#   tools/causticos/run.sh --list FILE      (one test source per line)
#
# Each test is built for --target=caustic-x86_64, put on a root image under /cm
# beside every testdata/ directory of the repository (the same relative paths
# the tests open on Linux), and run by tools/causticos/runall.cst as /init in
# one headless boot. The image is built by the causticos repository's own
# scripts (CAUSTICOS_DIR, default ../causticos), into $WORK — never into that
# repository's build/.
#
# Exit 0 when every test exited 0. The serial log is $WORK/serial.log.
set -u
here="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$here"
COS="${CAUSTICOS_DIR:-$here/../causticos}"
WORK="${WORK:-/tmp/cm-causticos}"
SECONDS_MAX="${SECONDS_MAX:-900}"
mkdir -p "$WORK/b" "$WORK/err"

tests=()
if [ "${1:-}" = "--list" ]; then
    mapfile -t tests < <(grep -v '^\s*\(#\|$\)' "$2")
else
    tests=("$@")
fi
[ ${#tests[@]} -gt 0 ] || { echo "usage: $0 test.cst ... | --list FILE" >&2; exit 2; }

build() {
    local src="$1" out="$2" log="$3"
    caustic -q -O2 --target=caustic-x86_64 "$src" -o "$out" --path .caustic/deps > "$log" 2>&1
}

build tools/causticos/runall.cst "$WORK/runall.cse" "$WORK/err/runall.txt" \
    || { cat "$WORK/err/runall.txt"; exit 1; }

# The tests, four at a time.
: > "$WORK/list"
export -f build
export WORK
printf '%s\n' "${tests[@]}" | xargs -P4 -I{} bash -c '
    n=$(echo "{}" | tr / _ | sed "s/\.cst$//")
    if build "{}" "$WORK/b/$n.cse" "$WORK/err/$n.txt"; then echo "built {}"; else echo "BUILD FAIL {} (see $WORK/err/$n.txt)"; fi
' | sort > "$WORK/build.txt"
grep -c '^built' "$WORK/build.txt" | sed 's/^/built: /'
grep '^BUILD FAIL' "$WORK/build.txt"

adds=(--add "$WORK/runall.cse:/init.cse")
for t in "${tests[@]}"; do
    n=$(echo "$t" | tr / _ | sed 's/\.cst$//')
    [ -f "$WORK/b/$n.cse" ] || continue
    adds+=(--add "$WORK/b/$n.cse:/cm/t/$n.cse")
    echo "/cm/t/$n.cse" >> "$WORK/list"
done
adds+=(--add "$WORK/list:/cm/tests")
while IFS= read -r f; do
    adds+=(--add "$here/$f:/cm/$f")
done < <(find . -path ./build -prune -o -path ./.caustic -prune -o -path '*/testdata/*' -type f -print | sed 's#^\./##')

rm -f "$WORK/test.iso"
( cd "$COS" && bash scripts/mkiso.sh --profile shell --size 160M "${adds[@]}" --out "$WORK/test.iso" ) \
    > "$WORK/mkiso.txt" 2>&1 || { cat "$WORK/mkiso.txt"; exit 1; }

# Booted until runall says DONE, or the deadline.
(
    cd "$COS"
    QEMU_ISO="$WORK/test.iso"; QEMU_KVM=1; QEMU_MEM="${QEMU_MEM:-768M}"
    source scripts/qemu-args.sh
    exec qemu-system-x86_64 "${QEMU_ARGS[@]}" -serial stdio -display none
) > "$WORK/serial.log" 2>&1 &
qpid=$!
start=$(date +%s)
while kill -0 $qpid 2>/dev/null; do
    tr -d '\000' < "$WORK/serial.log" | grep -q 'cmtests: DONE' && break
    [ $(( $(date +%s) - start )) -ge "$SECONDS_MAX" ] && { echo "deadline: no DONE after ${SECONDS_MAX}s"; break; }
    sleep 1
done
kill $qpid 2>/dev/null; wait $qpid 2>/dev/null

tr -d '\000' < "$WORK/serial.log" | grep -E '^cmtests: (<<<|[0-9])'
tr -d '\000' < "$WORK/serial.log" | grep -q 'cmtests: DONE' || exit 1
tr -d '\000' < "$WORK/serial.log" | grep -q 'cmtests: [0-9]* ran, 0 failed' || exit 1
exit 0
