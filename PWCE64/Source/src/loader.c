#include "loader.h"

#include "log.h"

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

#define PATH_LENGTH_MAX    512
#define MESSAGE_LENGTH_MAX 640

typedef int32 PluginEntry(const PwceApi* api);

static void log_plugin(const char* text, const char* name)
{
    char message[MESSAGE_LENGTH_MAX];

    snprintf(message, sizeof(message), "%s: %s", text, name);
    pwce_log(message);
}

static bool load_plugin(const char* path, const char* name, const PwceApi* api)
{
    HMODULE handle = LoadLibraryA(path);

    if (handle == NULL)
    {
        char message[MESSAGE_LENGTH_MAX];

        snprintf(message, sizeof(message), "plugin load failed: %s (error %lu)", name, GetLastError());
        pwce_log(message);

        return false;
    }

    PluginEntry* entry = (PluginEntry*)GetProcAddress(handle, "PwcePluginLoad");

    if (entry == NULL)
    {
        log_plugin("plugin has no PwcePluginLoad", name);
        FreeLibrary(handle);

        return false;
    }

    if (entry(api) != 0)
    {
        log_plugin("plugin load returned error", name);
        FreeLibrary(handle);

        return false;
    }

    log_plugin("plugin loaded", name);

    return true;
}

int32 pwce_loader_load_all(const char* directory, const PwceApi* api)
{
    char pattern[PATH_LENGTH_MAX];

    snprintf(pattern, sizeof(pattern), "%s\\*.dll", directory);

    WIN32_FIND_DATAA found;
    HANDLE handle = FindFirstFileA(pattern, &found);

    if (handle == INVALID_HANDLE_VALUE)
    {
        pwce_log("no plugins found");

        return 0;
    }

    int32 loaded = 0;

    do
    {
        char path[PATH_LENGTH_MAX];

        snprintf(path, sizeof(path), "%s\\%s", directory, found.cFileName);

        if (load_plugin(path, found.cFileName, api))
            loaded++;
    }
    while (FindNextFileA(handle, &found));

    FindClose(handle);

    return loaded;
}
