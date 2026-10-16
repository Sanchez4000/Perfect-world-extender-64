@echo off
setlocal
set "HERE=%~dp0"
if "%HERE:~-1%"=="\" set "HERE=%HERE:~0,-1%"
for %%I in ("%HERE%") do set "NAME=%%~nxI"
set "IMAGE=pwse64-toolchain"

echo [build] plugin: %NAME%
echo [build] checking toolchain image...
docker image inspect %IMAGE% >nul 2>&1
if errorlevel 1 echo [build] building toolchain image (first run)...
if errorlevel 1 docker build -t %IMAGE% "%HERE%\toolchain"
if errorlevel 1 echo [build] toolchain build FAILED
if errorlevel 1 exit /b 1

echo [build] compiling %NAME%.so ...
docker run --rm -v "%HERE%:/src" -w /src %IMAGE% sh -c "mkdir -p build && gcc -std=gnu11 -shared -fPIC -O2 -Wall -fno-stack-protector -Iinclude src/*.c -o build/%NAME%.so"
if errorlevel 1 echo [build] compile FAILED
if errorlevel 1 exit /b 1

echo [build] OK: build\%NAME%.so
