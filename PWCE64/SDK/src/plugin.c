#include "pwce.h"

static const PwceApi* pwce_api = NULL;

/*
typedef void SomeFunction(void* self, int32 value);

static SomeFunction* some_function_original = NULL;

static void some_function_hook(void* self, int32 value)
{
    pwce_api->Log("[myplugin] some_function");
    some_function_original(self, value);
}
*/

int32 PwcePluginLoad(const PwceApi* api)
{
    pwce_api = api;

    pwce_api->Log("[myplugin] loaded");

    /*
    if (pwce_api->Hook("some::Function", (void*)some_function_hook, (void**)&some_function_original) != 0)
        return -1;

    SomeFunction* some_function = (SomeFunction*)pwce_api->Resolve("some::Function");

    if (some_function != NULL)
        some_function(NULL, 0);
    */

    return 0;
}
