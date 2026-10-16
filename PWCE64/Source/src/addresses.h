#ifndef PWCE_ADDRESSES_H
#define PWCE_ADDRESSES_H

#include "types.h"

int32 pwce_addresses_load(const char* path);
uintptr pwce_addresses_resolve(const char* name);

#endif
