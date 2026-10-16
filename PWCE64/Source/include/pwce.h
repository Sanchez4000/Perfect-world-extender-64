#ifndef PWCE_H
#define PWCE_H

#include "types.h"

#define PWCE_ABI 1

#define PWCE_EXPORT __declspec(dllexport)

typedef struct PwceApi PwceApi;

struct PwceApi
{
    int32 abi_version;
    void (*Log)(const char* message);
    uintptr (*Resolve)(const char* name);
    int32 (*Hook)(const char* name, void* detour, void** original);
};

PWCE_EXPORT int32 PwcePluginLoad(const PwceApi* api);

#endif
