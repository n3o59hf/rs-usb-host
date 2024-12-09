#!/bin/bash

ORIGINAL_DIR=$(pwd)

git clone https://github.com/raspberrypi/pico-sdk

cd pico-sdk || exit
git fetch --tags
git checkout 1.5.1
git submodule update --init

cd "$ORIGINAL_DIR" || exit

cd "pico-sdk/lib/tinyusb" || exit
git fetch --tags
git checkout 0.17.0

cd "$ORIGINAL_DIR" || exit