#ifndef PWSE_ADDRESSES_H
#define PWSE_ADDRESSES_H

#include "types.h"

int32 pwse_addresses_load(const char* path);
uintptr pwse_addresses_resolve(const char* name);

#endif
