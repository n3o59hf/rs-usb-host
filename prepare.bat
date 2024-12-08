@echo off

set ORIGINAL_DIR=%CD%

git clone https://github.com/raspberrypi/pico-sdk

echo Current directory: %CD%
set PICO_SDK_PATH=%CD%\pico-sdk
cd /d %PICO_SDK_PATH% 
echo Current directory: %CD%
git fetch --tags
git checkout 1.5.1
git submodule update --init
cd %ORIGINAL_DIR%

echo Current directory: %CD%
cd /d %PICO_SDK_PATH%\lib\tinyusb
echo Current directory: %CD%

git fetch --tags
git checkout 0.17.0
cd %ORIGINAL_DIR%


