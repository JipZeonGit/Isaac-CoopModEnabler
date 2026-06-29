@echo off
setlocal

REM ============================================================
REM  CoopModEnabler - Build Script
REM  Requires Visual Studio with C++ Desktop workload
REM  Run from "Developer Command Prompt for VS" or "x86 Native Tools"
REM ============================================================

REM Try to find vcvarsall.bat if not already in a dev prompt
where cl >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [!] cl.exe not found. Trying to locate Visual Studio...

    REM VS 2022
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
        goto :build
    )
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" x86
        goto :build
    )
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
        goto :build
    )

    REM VS 2019
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
        goto :build
    )

    echo [ERROR] Could not find Visual Studio. Please run from a Developer Command Prompt.
    exit /b 1
)

:build
echo.
echo [*] Building CoopModEnabler (winmm.dll) - 32-bit Release
echo.

if not exist "build" mkdir build

cl /nologo /O2 /W3 /LD /Fe:build\winmm.dll ^
    src\dllmain.c src\patcher.c src\winmm_proxy.c ^
    /DEF:exports.def ^
    /link /NOLOGO /DLL /MACHINE:X86 ^
    kernel32.lib user32.lib

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [OK] Build successful! Output: build\winmm.dll
    echo [*]  Copy build\winmm.dll to your game directory ^(next to isaac-ng.exe^)
    
    REM Clean up intermediate files
    if exist winmm.exp del winmm.exp
    if exist winmm.lib del winmm.lib
    move /Y winmm.obj build\ >nul 2>&1
    move /Y dllmain.obj build\ >nul 2>&1
    move /Y patcher.obj build\ >nul 2>&1
    move /Y winmm_proxy.obj build\ >nul 2>&1
) else (
    echo.
    echo [ERROR] Build failed!
)

endlocal
