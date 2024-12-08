@echo off

set ORIGINAL_DIR=%CD%

set PICO_SDK_PATH=%CD%\pico-sdk

if not exist build (
    mkdir build
)

cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release  ..

make -j32
