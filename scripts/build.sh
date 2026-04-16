#!/usr/bin/env bash
set -euo pipefail

preset=$1

printf "[Build] Running pre-build commands\n"


printf "[Build] Configuring preset: %s\n" "$preset"
cmake --preset "$preset"

printf "[Build] Building preset: %s\n" "$preset"
cmake --build --preset "$preset"

ASSETS_SRC="$(pwd)/assets"
ASSETS_DST="./build/$preset/assets"

if [[ "$preset" == "dev" ]]; then
  if [[ ! -L "$ASSETS_DST" ]]; then
    rm -rf "$ASSETS_DST"
    ln -s "$ASSETS_SRC" "$ASSETS_DST"
    printf "[Build] Created assets symlink\n"
  fi
fi

printf "[Build] Done!\n"
