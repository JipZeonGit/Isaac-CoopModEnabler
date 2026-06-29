/*
 * CoopModEnabler - Memory Patcher
 *
 * Scans the loaded isaac-ng.exe image in memory and applies byte patches
 * to remove the online co-op mod restriction and disable analytics.
 *
 * Patch logic is identical to the original C# GamePatcher:
 *   - Patch 1: NOP out three JE instructions in the mod-check switch/case
 *   - Patch 2: Replace PUSH EBP with RET at analytics function entry
 */

#include "patcher.h"
#include <string.h>

/* =========================================================================
 * Byte Patterns (must match GamePatcher.cs exactly)
 * ========================================================================= */

/*
 * Patch 1: Online co-op mod restriction
 *
 * Original:  83 E8 02       sub eax, 2
 *            74 2A          je  +0x2A      <-- NOP this
 *            83 E8 01       sub eax, 1
 *            74 1E          je  +0x1E      <-- NOP this
 *            83 E8 01       sub eax, 1
 *            74 12          je  +0x12      <-- NOP this
 *            32 C0          xor al, al
 *            8B 4D F4       mov ecx, [ebp-0Ch]
 *            64 89 0D 00000000  mov fs:[0], ecx
 */
static const BYTE PATTERN_MOD_CHECK[] = {
    0x83, 0xE8, 0x02,
    0x74, 0x2A,
    0x83, 0xE8, 0x01,
    0x74, 0x1E,
    0x83, 0xE8, 0x01,
    0x74, 0x12,
    0x32, 0xC0,
    0x8B, 0x4D, 0xF4,
    0x64, 0x89, 0x0D, 0x00, 0x00, 0x00, 0x00
};

/* Already-patched version (JE replaced with NOP NOP) */
static const BYTE PATTERN_MOD_CHECK_DONE[] = {
    0x83, 0xE8, 0x02,
    0x90, 0x90,
    0x83, 0xE8, 0x01,
    0x90, 0x90,
    0x83, 0xE8, 0x01,
    0x90, 0x90,
    0x32, 0xC0,
    0x8B, 0x4D, 0xF4,
    0x64, 0x89, 0x0D, 0x00, 0x00, 0x00, 0x00
};

/*
 * Patch 2: Analytics / telemetry function
 *
 * Original:  55             push ebp        <-- Replace with RET (0xC3)
 *            8B EC          mov ebp, esp
 *            83 EC 10       sub esp, 10h
 *            53             push ebx
 *            56             push esi
 *            57             push edi
 *            FF 15 xx       call [addr]
 */
static const BYTE PATTERN_ANALYTICS[] = {
    0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x10, 0x53, 0x56, 0x57, 0xFF, 0x15
};

/* Already-patched version (PUSH EBP -> RET) */
static const BYTE PATTERN_ANALYTICS_DONE[] = {
    0xC3, 0x8B, 0xEC, 0x83, 0xEC, 0x10, 0x53, 0x56, 0x57, 0xFF, 0x15
};

/* =========================================================================
 * Pattern Scanner
 * ========================================================================= */

static BYTE* FindPattern(BYTE *base, SIZE_T size, const BYTE *pattern, SIZE_T patternLen) {
    if (patternLen > size) return NULL;

    SIZE_T limit = size - patternLen;
    for (SIZE_T i = 0; i <= limit; i++) {
        if (memcmp(&base[i], pattern, patternLen) == 0) {
            return &base[i];
        }
    }
    return NULL;
}

/* =========================================================================
 * Patch Application
 * ========================================================================= */

BOOL Patcher_Apply(void) {
    /* Get the base address of the main executable (isaac-ng.exe) */
    HMODULE hExe = GetModuleHandleA(NULL);
    if (!hExe) return FALSE;

    /* Parse PE headers to determine image size */
    BYTE *baseAddr = (BYTE *)hExe;
    PIMAGE_DOS_HEADER dosHdr = (PIMAGE_DOS_HEADER)baseAddr;
    if (dosHdr->e_magic != IMAGE_DOS_SIGNATURE) return FALSE;

    PIMAGE_NT_HEADERS ntHdr = (PIMAGE_NT_HEADERS)(baseAddr + dosHdr->e_lfanew);
    if (ntHdr->Signature != IMAGE_NT_SIGNATURE) return FALSE;

    DWORD imageSize = ntHdr->OptionalHeader.SizeOfImage;
    BOOL anyPatched = FALSE;

    /* --- Patch 1: Remove online co-op mod restriction --- */
    {
        BYTE *addr = FindPattern(baseAddr, imageSize,
                                 PATTERN_MOD_CHECK, sizeof(PATTERN_MOD_CHECK));
        if (addr) {
            DWORD oldProtect;
            VirtualProtect(addr, sizeof(PATTERN_MOD_CHECK),
                           PAGE_EXECUTE_READWRITE, &oldProtect);

            /* NOP out three JE (conditional jump) instructions */
            addr[3]  = 0x90;  addr[4]  = 0x90;   /* je +0x2A -> NOP NOP */
            addr[8]  = 0x90;  addr[9]  = 0x90;   /* je +0x1E -> NOP NOP */
            addr[13] = 0x90;  addr[14] = 0x90;   /* je +0x12 -> NOP NOP */

            VirtualProtect(addr, sizeof(PATTERN_MOD_CHECK),
                           oldProtect, &oldProtect);
            anyPatched = TRUE;
        } else {
            /* Check if already patched */
            if (FindPattern(baseAddr, imageSize,
                            PATTERN_MOD_CHECK_DONE, sizeof(PATTERN_MOD_CHECK_DONE))) {
                anyPatched = TRUE;  /* Already good */
            }
        }
    }

    /* --- Patch 2: Disable analytics reporting --- */
    {
        BYTE *addr = FindPattern(baseAddr, imageSize,
                                 PATTERN_ANALYTICS, sizeof(PATTERN_ANALYTICS));
        if (addr) {
            DWORD oldProtect;
            VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);

            addr[0] = 0xC3;  /* PUSH EBP -> RET */

            VirtualProtect(addr, 1, oldProtect, &oldProtect);
            anyPatched = TRUE;
        } else {
            if (FindPattern(baseAddr, imageSize,
                            PATTERN_ANALYTICS_DONE, sizeof(PATTERN_ANALYTICS_DONE))) {
                anyPatched = TRUE;
            }
        }
    }

    return anyPatched;
}
