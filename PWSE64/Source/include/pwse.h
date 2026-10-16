#ifndef PWSE_H
#define PWSE_H

#include "types.h"

#define PWSE_ABI 1

typedef struct PwseApi PwseApi;

struct PwseApi
{
    int32 abi_version;
    void (*Log)(const char* message);
    uintptr (*Resolve)(const char* name);
    int32 (*Hook)(const char* name, void* detour, void** original);
};

int32 PwsePluginLoad(const PwseApi* api);

#endif
