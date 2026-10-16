@echo off
setlocal
set "HERE=%~dp0"
if "%HERE:~-1%"=="\" set "HERE=%HERE:~0,-1%"
for %%I in ("%HERE%") do set "NAME=%%~nxI"

where gcc >nul 2>&1
if errorlevel 1 echo [build] gcc not found in PATH, mingw-w64 x86_64 is required
if errorlevel 1 exit /b 1

if not exist "%HERE%\build" mkdir "%HERE%\build"

echo [build] compiling %NAME%.dll ...
gcc -std=gnu11 -shared -O2 -Wall -static -fno-stack-protector -I"%HERE%\include" "%HERE%\src\plugin.c" -o "%HERE%\build\%NAME%.dll"
if errorlevel 1 echo [build] compile FAILED
if errorlevel 1 exit /b 1

echo [build] OK: build\%NAME%.dll
