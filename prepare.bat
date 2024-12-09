@echo off

set ORIGINAL_DIR=%CD%

git clone https://github.com/raspberrypi/pico-sdk

cd /d pico-sdk
git fetch --tags
git checkout 1.5.1
git submodule update --init

cd %ORIGINAL_DIR%

cd /d pico-sdk\lib\tinyusb
git fetch --tags
git checkout 0.17.0

cd %ORIGINAL_DIR%


