#pragma once
#ifndef WINMM_PROXY_H
#define WINMM_PROXY_H

#include <windows.h>

/*
 * Load the real system winmm.dll and resolve all function pointers.
 * Must be called before any proxied winmm function is invoked.
 */
BOOL WinmmProxy_Init(void);

/*
 * Unload the original winmm.dll handle.
 */
void WinmmProxy_Cleanup(void);

#endif /* WINMM_PROXY_H */
