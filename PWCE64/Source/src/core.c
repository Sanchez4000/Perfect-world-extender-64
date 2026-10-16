#include "pwce.h"

#include "addresses.h"
#include "hooks.h"
#include "loader.h"
#include "log.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define PATH_LENGTH_MAX      512
#define DIRECTORY_LENGTH_MAX 256

#define LOG_FILE       "pwce.log"
#define PLUGINS_FOLDER "plugins"
#define ADDRESSES_FILE "plugins\\_addresses.ini"

static int32 api_hook(const char* name, void* detour, void** original)
{
    const uintptr address = pwce_addresses_resolve(name);

    if (address == 0)
    {
        pwce_log("hook: name not found in address library");

        return -1;
    }

    return pwce_hook_install(address, detour, original);
}

static const PwceApi api =
{
    .abi_version = PWCE_ABI,
    .Log = pwce_log,
    .Resolve = pwce_addresses_resolve,
    .Hook = api_hook
};

static bool module_directory(HMODULE module, char* directory, size_t size)
{
    const DWORD length = GetModuleFileNameA(module, directory, (DWORD)size);

    if (length == 0 || length >= size)
        return false;

    char* separator = strrchr(directory, '\\');

    if (separator == NULL)
        return false;

    *separator = 0;

    return true;
}

static void join_path(char* path, const char* directory, const char* name)
{
    snprintf(path, PATH_LENGTH_MAX, "%s\\%s", directory, name);
}

static void start(HMODULE module)
{
    char directory[DIRECTORY_LENGTH_MAX];
    char path[PATH_LENGTH_MAX];

    if (!module_directory(module, directory, sizeof(directory)))
        return;

    join_path(path, directory, LOG_FILE);
    pwce_log_open(path);
    pwce_log("loader start");

    join_path(path, directory, ADDRESSES_FILE);

    if (pwce_addresses_load(path) < 0)
        pwce_log("address library not found");

    join_path(path, directory, PLUGINS_FOLDER);
    pwce_loader_load_all(path, &api);

    pwce_log("loader done");
}

// Runs under the loader lock before the engine starts: plugins may only hook functions and must not wait on other threads.
BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID reserved)
{
    (void)reserved;

    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(module);
        start(module);
    }

    return TRUE;
}
