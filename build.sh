#!/bin/bash

ORIGINAL_DIR=$(pwd)

PICO_SDK_PATH="$ORIGINAL_DIR/pico-sdk"

if [ ! -d build ]; then
    mkdir build
fi

cd build || exit

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release  ..

NUM_CORES=$(nproc)
make -j"$NUM_CORES"

cd "$ORIGINAL_DIR" || exit