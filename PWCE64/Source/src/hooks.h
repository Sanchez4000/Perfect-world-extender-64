#ifndef PWCE_HOOKS_H
#define PWCE_HOOKS_H

#include "types.h"

int32 pwce_hook_install(uintptr address, void* detour, void** original);

#endif
