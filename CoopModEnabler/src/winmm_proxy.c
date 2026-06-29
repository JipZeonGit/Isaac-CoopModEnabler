/*
 * CoopModEnabler - winmm.dll Proxy Forwarding
 *
 * Loads the real system winmm.dll and forwards all API calls to it.
 * Uses X-macro technique to generate 178 proxy trampolines from a single list.
 *
 * Each proxy function is a naked assembly trampoline that does an indirect
 * JMP to the real function, preserving the caller's stack frame and
 * calling convention exactly.
 */

#include "winmm_proxy.h"
#include <stdio.h>

/* =========================================================================
 * X-Macro: Complete list of winmm.dll exported functions
 *
 * This single list generates:
 *   - Function pointer declarations
 *   - Proxy trampoline functions
 *   - GetProcAddress initialization calls
 * ========================================================================= */

#define WINMM_FUNCTIONS \
    X(CloseDriver) \
    X(DefDriverProc) \
    X(DriverCallback) \
    X(DrvGetModuleHandle) \
    X(GetDriverModuleHandle) \
    X(OpenDriver) \
    X(PlaySoundA) \
    X(PlaySoundW) \
    X(SendDriverMessage) \
    X(WOWAppExit) \
    X(auxGetDevCapsA) \
    X(auxGetDevCapsW) \
    X(auxGetNumDevs) \
    X(auxGetVolume) \
    X(auxOutMessage) \
    X(auxSetVolume) \
    X(joyConfigChanged) \
    X(joyGetDevCapsA) \
    X(joyGetDevCapsW) \
    X(joyGetNumDevs) \
    X(joyGetPos) \
    X(joyGetPosEx) \
    X(joyGetThreshold) \
    X(joyReleaseCapture) \
    X(joySetCapture) \
    X(joySetThreshold) \
    X(mciDriverNotify) \
    X(mciDriverYield) \
    X(mciFreeCommandResource) \
    X(mciGetCreatorTask) \
    X(mciGetDeviceIDA) \
    X(mciGetDeviceIDFromElementIDA) \
    X(mciGetDeviceIDFromElementIDW) \
    X(mciGetDeviceIDW) \
    X(mciGetDriverData) \
    X(mciGetErrorStringA) \
    X(mciGetErrorStringW) \
    X(mciGetYieldProc) \
    X(mciLoadCommandResource) \
    X(mciSendCommandA) \
    X(mciSendCommandW) \
    X(mciSendStringA) \
    X(mciSendStringW) \
    X(mciSetDriverData) \
    X(mciSetYieldProc) \
    X(midiConnect) \
    X(midiDisconnect) \
    X(midiInAddBuffer) \
    X(midiInClose) \
    X(midiInGetDevCapsA) \
    X(midiInGetDevCapsW) \
    X(midiInGetErrorTextA) \
    X(midiInGetErrorTextW) \
    X(midiInGetID) \
    X(midiInGetNumDevs) \
    X(midiInMessage) \
    X(midiInOpen) \
    X(midiInPrepareHeader) \
    X(midiInReset) \
    X(midiInStart) \
    X(midiInStop) \
    X(midiInUnprepareHeader) \
    X(midiOutCacheDrumPatches) \
    X(midiOutCachePatches) \
    X(midiOutClose) \
    X(midiOutGetDevCapsA) \
    X(midiOutGetDevCapsW) \
    X(midiOutGetErrorTextA) \
    X(midiOutGetErrorTextW) \
    X(midiOutGetID) \
    X(midiOutGetNumDevs) \
    X(midiOutGetVolume) \
    X(midiOutLongMsg) \
    X(midiOutMessage) \
    X(midiOutOpen) \
    X(midiOutPrepareHeader) \
    X(midiOutReset) \
    X(midiOutSetVolume) \
    X(midiOutShortMsg) \
    X(midiOutUnprepareHeader) \
    X(midiStreamClose) \
    X(midiStreamOpen) \
    X(midiStreamOut) \
    X(midiStreamPause) \
    X(midiStreamPosition) \
    X(midiStreamProperty) \
    X(midiStreamRestart) \
    X(midiStreamStop) \
    X(mixerClose) \
    X(mixerGetControlDetailsA) \
    X(mixerGetControlDetailsW) \
    X(mixerGetDevCapsA) \
    X(mixerGetDevCapsW) \
    X(mixerGetID) \
    X(mixerGetLineControlsA) \
    X(mixerGetLineControlsW) \
    X(mixerGetLineInfoA) \
    X(mixerGetLineInfoW) \
    X(mixerGetNumDevs) \
    X(mixerMessage) \
    X(mixerOpen) \
    X(mixerSetControlDetails) \
    X(mmDrvInstall) \
    X(mmGetCurrentTask) \
    X(mmTaskBlock) \
    X(mmTaskCreate) \
    X(mmTaskSignal) \
    X(mmTaskYield) \
    X(mmioAdvance) \
    X(mmioAscend) \
    X(mmioClose) \
    X(mmioCreateChunk) \
    X(mmioDescend) \
    X(mmioFlush) \
    X(mmioGetInfo) \
    X(mmioInstallIOProcA) \
    X(mmioInstallIOProcW) \
    X(mmioOpenA) \
    X(mmioOpenW) \
    X(mmioRead) \
    X(mmioRenameA) \
    X(mmioRenameW) \
    X(mmioSeek) \
    X(mmioSendMessage) \
    X(mmioSetBuffer) \
    X(mmioSetInfo) \
    X(mmioStringToFOURCCA) \
    X(mmioStringToFOURCCW) \
    X(mmioWrite) \
    X(mmsystemGetVersion) \
    X(sndPlaySoundA) \
    X(sndPlaySoundW) \
    X(timeBeginPeriod) \
    X(timeEndPeriod) \
    X(timeGetDevCaps) \
    X(timeGetSystemTime) \
    X(timeGetTime) \
    X(timeKillEvent) \
    X(timeSetEvent) \
    X(waveInAddBuffer) \
    X(waveInClose) \
    X(waveInGetDevCapsA) \
    X(waveInGetDevCapsW) \
    X(waveInGetErrorTextA) \
    X(waveInGetErrorTextW) \
    X(waveInGetID) \
    X(waveInGetNumDevs) \
    X(waveInGetPosition) \
    X(waveInMessage) \
    X(waveInOpen) \
    X(waveInPrepareHeader) \
    X(waveInReset) \
    X(waveInStart) \
    X(waveInStop) \
    X(waveInUnprepareHeader) \
    X(waveOutBreakLoop) \
    X(waveOutClose) \
    X(waveOutGetDevCapsA) \
    X(waveOutGetDevCapsW) \
    X(waveOutGetErrorTextA) \
    X(waveOutGetErrorTextW) \
    X(waveOutGetID) \
    X(waveOutGetNumDevs) \
    X(waveOutGetPitch) \
    X(waveOutGetPlaybackRate) \
    X(waveOutGetPosition) \
    X(waveOutGetVolume) \
    X(waveOutMessage) \
    X(waveOutOpen) \
    X(waveOutPause) \
    X(waveOutPrepareHeader) \
    X(waveOutReset) \
    X(waveOutRestart) \
    X(waveOutSetPitch) \
    X(waveOutSetPlaybackRate) \
    X(waveOutSetVolume) \
    X(waveOutUnprepareHeader) \
    X(waveOutWrite)

/* =========================================================================
 * Generate function pointer storage
 * ========================================================================= */

#define X(name) FARPROC pfn_##name = NULL;
WINMM_FUNCTIONS
#undef X

/* =========================================================================
 * Generate proxy trampoline functions
 *
 * Each proxy is a naked function that does a direct JMP to the real
 * winmm function via its function pointer. This preserves the stack
 * layout and calling convention perfectly.
 * ========================================================================= */

#ifdef _MSC_VER
    /* MSVC x86: use __declspec(naked) + inline asm */
    #define X(name) \
        __declspec(naked) void proxy_##name(void) { \
            __asm { jmp dword ptr [pfn_##name] } \
        }
#elif defined(__GNUC__) || defined(__clang__)
    /* GCC / Clang / MinGW / Zig CC x86: use __attribute__((naked)) + GAS syntax */
    #define X(name) \
        __attribute__((naked)) void proxy_##name(void) { \
            __asm__ volatile ("jmp *_pfn_" #name); \
        }
#else
    #error "Unsupported compiler. Use MSVC, GCC/MinGW, or Clang/Zig."
#endif

WINMM_FUNCTIONS
#undef X

/* =========================================================================
 * Initialization & Cleanup
 * ========================================================================= */

static HMODULE g_hOriginalWinmm = NULL;

BOOL WinmmProxy_Init(void) {
    /* Build path to the real system winmm.dll */
    char systemDir[MAX_PATH];
    GetSystemDirectoryA(systemDir, MAX_PATH);
    strcat_s(systemDir, MAX_PATH, "\\winmm.dll");

    /* Load the original DLL */
    g_hOriginalWinmm = LoadLibraryA(systemDir);
    if (!g_hOriginalWinmm) {
        return FALSE;
    }

    /* Resolve all function pointers */
    #define X(name) pfn_##name = GetProcAddress(g_hOriginalWinmm, #name);
    WINMM_FUNCTIONS
    #undef X

    return TRUE;
}

void WinmmProxy_Cleanup(void) {
    if (g_hOriginalWinmm) {
        FreeLibrary(g_hOriginalWinmm);
        g_hOriginalWinmm = NULL;
    }
}
