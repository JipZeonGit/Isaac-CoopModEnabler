#pragma once
#ifndef PATCHER_H
#define PATCHER_H

#include <windows.h>

/*
 * Apply all memory patches to isaac-ng.exe:
 *   1. NOP out conditional jumps that disable mods in online co-op
 *   2. Replace analytics function entry with RET to skip telemetry
 *
 * Returns TRUE if at least one patch was applied or already present.
 */
BOOL Patcher_Apply(void);

#endif /* PATCHER_H */
