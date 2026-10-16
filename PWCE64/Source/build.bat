@echo off
setlocal
set "HERE=%~dp0"
if "%HERE:~-1%"=="\" set "HERE=%HERE:~0,-1%"

where gcc >nul 2>&1
if errorlevel 1 echo [build] gcc not found in PATH, mingw-w64 x86_64 is required
if errorlevel 1 exit /b 1

if not exist "%HERE%\build" mkdir "%HERE%\build"

echo [build] compiling FTDriver_64.dll ...
gcc -std=gnu11 -shared -O2 -Wall -static -fno-stack-protector -DSUBHOOK_STATIC -I"%HERE%\include" -I"%HERE%\src" -I"%HERE%\vendor\subhook" "%HERE%\src\core.c" "%HERE%\src\log.c" "%HERE%\src\addresses.c" "%HERE%\src\hooks.c" "%HERE%\src\loader.c" "%HERE%\vendor\subhook\subhook.c" "%HERE%\FTDriver_64.def" -o "%HERE%\build\FTDriver_64.dll"
if errorlevel 1 echo [build] compile FAILED
if errorlevel 1 exit /b 1

echo [build] OK: build\FTDriver_64.dll
