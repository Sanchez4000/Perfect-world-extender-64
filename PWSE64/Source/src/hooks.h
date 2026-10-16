#ifndef PWSE_HOOKS_H
#define PWSE_HOOKS_H

#include "types.h"

int32 pwse_hook_install(uintptr address, void* detour, void** original);

#endif
