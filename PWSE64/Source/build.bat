@echo off
setlocal
set "HERE=%~dp0"
if "%HERE:~-1%"=="\" set "HERE=%HERE:~0,-1%"
set "IMAGE=pwse64-toolchain"

echo [build] checking toolchain image...
docker image inspect %IMAGE% >nul 2>&1
if errorlevel 1 echo [build] building toolchain image (first run)...
if errorlevel 1 docker build -t %IMAGE% "%HERE%\toolchain"
if errorlevel 1 echo [build] toolchain build FAILED
if errorlevel 1 exit /b 1

echo [build] compiling pwse_loader.so ...
docker run --rm -v "%HERE%:/src" -w /src %IMAGE% sh -c "mkdir -p build && gcc -std=gnu11 -shared -fPIC -O2 -Wall -Wno-unused-result -fno-stack-protector -DSUBHOOK_STATIC -Iinclude -Isrc -Ivendor/subhook src/core.c src/log.c src/addresses.c src/hooks.c src/loader.c vendor/subhook/subhook.c -ldl -o build/pwse_loader.so"
if errorlevel 1 echo [build] compile FAILED
if errorlevel 1 exit /b 1

echo [build] OK: build\pwse_loader.so
