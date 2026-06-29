@echo off
setlocal

REM ============================================================
REM  CoopModEnabler - Build Script (Zig CC)
REM  Uses Zig's built-in C compiler for cross-compilation to x86
REM ============================================================

set ZIG=zig

REM Check if zig is in PATH
where zig >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    REM Try local zig in build directory
    if exist "build\zig\zig.exe" (
        set ZIG=build\zig\zig.exe
    ) else (
        echo [ERROR] Zig not found. Please install Zig or extract it to build\zig\
        echo         Download from: https://ziglang.org/download/
        exit /b 1
    )
)

echo.
echo [*] Building CoopModEnabler (winmm.dll) - 32-bit Release
echo [*] Using: %ZIG%
echo.

if not exist "build" mkdir build

%ZIG% cc -target x86-windows-gnu -shared -O2 ^
    -o build\winmm.dll ^
    src\dllmain.c src\patcher.c src\winmm_proxy.c ^
    exports.def ^
    -lkernel32 -luser32

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [OK] Build successful! Output: build\winmm.dll
    echo [*]  Copy build\winmm.dll to your game directory ^(next to isaac-ng.exe^)
) else (
    echo.
    echo [ERROR] Build failed!
)

endlocal
