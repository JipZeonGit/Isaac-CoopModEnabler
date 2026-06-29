/*
 * CoopModEnabler - DLL Entry Point
 *
 * This DLL masquerades as winmm.dll (Windows Multimedia API).
 * When loaded by isaac-ng.exe, it:
 *   1. Forwards all winmm API calls to the real system winmm.dll
 *   2. Patches game memory to allow mods in online co-op
 */

#include <windows.h>
#include "patcher.h"
#include "winmm_proxy.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)hinstDLL;
    (void)lpvReserved;

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);

            /* Initialize proxy forwarding to real winmm.dll */
            if (!WinmmProxy_Init()) {
                /* If we can't load the real winmm.dll, abort */
                return FALSE;
            }

            /* Apply memory patches to enable mods in online co-op */
            Patcher_Apply();
            break;

        case DLL_PROCESS_DETACH:
            WinmmProxy_Cleanup();
            break;
    }

    return TRUE;
}
