#!/bin/sh
# ui/tools/api.sh — the toolkit's reference (ui/api.md), written from its
# sources: every module's first line, and each of its public functions —
# those whose names do not begin with an underscore — with the first
# sentence of the comment above it.
#
#   sh ui/tools/api.sh > ui/api.md        written again
#   sh ui/tools/api.sh | diff ui/api.md - kept current (the test script)
#
# Run from the repository's top.
set -eu
export LC_ALL=C

cat <<'EOF'
# The UI layer: reference

Written from the sources by `ui/tools/api.sh`; do not edit by hand. Every
module, and every public function in it with the first sentence of what its
comment says. The design is [`ui.md`](ui.md); what is built and tested,
[`state.md`](state.md).

A program reaches all of it through the hub, `use "ui/ui.cst" as ui;`:
`ui.<module>.<function>`, and the hub's own forms as `ui.<function>`.
EOF

for f in ui/ui.cst $(ls ui/*.cst | grep -v -e '_test\.cst$' -e '^ui/ui\.cst$' | sort) \
         $(ls ui/widgets/*.cst | grep -v '_test\.cst$' | sort); do
    awk -v file="$f" '
    function flush_sentence(s,    i, out) {
        gsub(/[ \t]+/, " ", s)
        sub(/^ /, "", s)
        i = index(s, ". ")
        if (i > 0) { s = substr(s, 1, i) }
        if (length(s) > 220) { s = substr(s, 1, 217) "..." }
        return s
    }
    NR == 1 {
        title = $0
        sub(/^\/\/ [^ ]+ — /, "", title)
        head = 1
        next
    }
    head == 1 && /^\/\/ ?[^ ]/ && title !~ /\.$/ {
        line = $0
        sub(/^\/\/ ?/, "", line)
        title = title " " line
        next
    }
    head == 1 {
        head = 0
        printf "\n## `%s`\n\n%s\n\n", file, flush_sentence(title)
    }
    /^\/\// {
        line = $0
        sub(/^\/\/ ?/, "", line)
        if (line ~ /^ *$/) { comment = ""; next }
        comment = comment " " line
        next
    }
    /^fn [a-z][A-Za-z0-9_]*\(/ {
        sig = $0
        sub(/ *\{.*$/, "", sig)
        sub(/^fn /, "", sig)
        c = flush_sentence(comment)
        if (c != "") { printf "- `%s` — %s\n", sig, c } else { printf "- `%s`\n", sig }
        comment = ""
        next
    }
    { comment = "" }
    ' "$f"
done
