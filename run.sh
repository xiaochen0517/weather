#!/bin/zsh

set -e

# get run script command args list
ARGS=("$@")

# shellcheck disable=SC2046
cd $(dirname "$0")

# build
cd ../
meson setup builddir --reconfigure
meson compile weather -j 14 -C builddir

# run
echo "========== Running Weather App =========="
export W_API_KID=XXXX
export W_API_SUB=XXXX
export W_DEBUG_MODE=1
./builddir/weather "${ARGS[@]}"
echo "========================================="
