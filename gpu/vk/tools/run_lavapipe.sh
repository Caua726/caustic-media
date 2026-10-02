#!/bin/sh
# gpu/vk/tools/run_lavapipe.sh — Vulkan on a machine with no GPU.
#
#   run_lavapipe.sh <programa> [args...]
#
# CI runners have no graphics hardware and no ICD, so a Vulkan test there does
# not fail for an interesting reason — vkCreateInstance simply reports that no
# driver is installed. lavapipe is Mesa's software Vulkan implementation, and it
# is what makes the binding and the backend testable in the same place the rest
# of the suite runs.
#
# --- Why the ICD is named rather than left to the loader ---
#
# On a developer machine there is usually a real driver installed as well, and
# the loader would pick it. That is the right default for a person and the wrong
# one for a test: the point of this script is to exercise the software path on
# purpose, so the same binary can be checked against llvmpipe here and against
# the hardware next to it. VK_ICD_FILENAMES names exactly one, and the loader
# then enumerates only that.
#
# --- Why it does not fall back silently ---
#
# A test that quietly runs on whatever driver it found reports a pass that means
# something different every time. If lavapipe is not installed this exits 127
# and says which package provides it, the same way run_headless.sh does for Xvfb.
set -eu

[ $# -ge 1 ] || { echo "uso: run_lavapipe.sh <programa> [args...]" >&2; exit 2; }

# Distributions disagree on the path, and the filename has been both
# lvp_icd.<arch>.json and lvp_icd.json across Mesa releases.
icd=""
for dir in /usr/share/vulkan/icd.d /usr/local/share/vulkan/icd.d \
           /etc/vulkan/icd.d /usr/lib/x86_64-linux-gnu/vulkan/icd.d; do
    for name in "lvp_icd.x86_64.json" "lvp_icd.json"; do
        [ -f "$dir/$name" ] && { icd="$dir/$name"; break; }
    done
    [ -n "$icd" ] && break
done

if [ -z "$icd" ]; then
    echo "run_lavapipe: lavapipe nao encontrado." >&2
    echo "  arch:   pacote vulkan-swrast" >&2
    echo "  debian: pacote mesa-vulkan-drivers" >&2
    exit 127
fi

# VK_DRIVER_FILES is the current name and VK_ICD_FILENAMES the one older loaders
# read. Setting both costs nothing and keeps this working across the range of
# loaders CI and developer machines actually have.
VK_DRIVER_FILES="$icd" VK_ICD_FILENAMES="$icd" exec "$@"
