#define _GNU_SOURCE
#include "pwse.h"

#include "addresses.h"
#include "hooks.h"
#include "loader.h"
#include "log.h"

#include <stdlib.h>

#define PLUGINS_DIRECTORY "plugins"
#define ADDRESSES_FILE    "plugins/_addresses.ini"

static int32 api_hook(const char* name, void* detour, void** original)
{
    const uintptr address = pwse_addresses_resolve(name);

    if (address == 0)
    {
        pwse_log("hook: name not found in address library");

        return -1;
    }

    return pwse_hook_install(address, detour, original);
}

static const PwseApi api =
{
    .abi_version = PWSE_ABI,
    .Log = pwse_log,
    .Resolve = pwse_addresses_resolve,
    .Hook = api_hook
};

__attribute__((constructor))
static void pwse_init(void)
{
    // Without this every child process of the engine would load the loader and its plugins again.
    unsetenv("LD_PRELOAD");

    pwse_log("loader start");

    if (pwse_addresses_load(ADDRESSES_FILE) < 0)
        pwse_log("address library not found");

    pwse_loader_load_all(PLUGINS_DIRECTORY, &api);

    pwse_log("loader done");
}
