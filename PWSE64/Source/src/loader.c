#define _GNU_SOURCE
#include "loader.h"

#include "log.h"

#include <dirent.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define PATH_LENGTH_MAX    512
#define MESSAGE_LENGTH_MAX 640

typedef int32 PluginEntry(const PwseApi* api);

static bool ends_with_so(const char* name)
{
    const size_t length = strlen(name);

    return length > 3 && strcmp(name + length - 3, ".so") == 0;
}

static void log_plugin(const char* text, const char* name)
{
    char message[MESSAGE_LENGTH_MAX];

    snprintf(message, sizeof(message), "%s: %s", text, name);
    pwse_log(message);
}

static bool load_plugin(const char* path, const char* name, const PwseApi* api)
{
    void* handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);

    if (handle == NULL)
    {
        char message[MESSAGE_LENGTH_MAX];

        snprintf(message, sizeof(message), "plugin dlopen failed: %s (%s)", name, dlerror());
        pwse_log(message);

        return false;
    }

    PluginEntry* entry = (PluginEntry*)dlsym(handle, "PwsePluginLoad");

    if (entry == NULL)
    {
        log_plugin("plugin has no PwsePluginLoad", name);
        dlclose(handle);

        return false;
    }

    if (entry(api) != 0)
    {
        log_plugin("plugin load returned error", name);
        dlclose(handle);

        return false;
    }

    log_plugin("plugin loaded", name);

    return true;
}

int32 pwse_loader_load_all(const char* directory, const PwseApi* api)
{
    DIR* handle = opendir(directory);

    if (handle == NULL)
    {
        pwse_log("plugins directory not found");

        return 0;
    }

    int32 loaded = 0;
    struct dirent* entry;

    while ((entry = readdir(handle)) != NULL)
    {
        if (!ends_with_so(entry->d_name))
            continue;

        char path[PATH_LENGTH_MAX];

        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

        if (load_plugin(path, entry->d_name, api))
            loaded++;
    }

    closedir(handle);

    return loaded;
}
