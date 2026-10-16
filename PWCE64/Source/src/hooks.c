#include "hooks.h"

#include "subhook.h"

// The trampoline can be allocated farther than a 5-byte relative jump reaches, so a 64-bit jump is required.
#define HOOK_FLAGS ((subhook_flags_t)(SUBHOOK_TRAMPOLINE | SUBHOOK_64BIT_OFFSET))

int32 pwce_hook_install(uintptr address, void* detour, void** original)
{
    if (address == 0 || detour == NULL)
        return -1;

    subhook_t hook = subhook_new((void*)address, detour, HOOK_FLAGS);

    if (hook == NULL)
        return -2;

    if (subhook_install(hook) != 0)
    {
        subhook_free(hook);

        return -3;
    }

    if (original == NULL)
        return 0;

    void* trampoline = subhook_get_trampoline(hook);

    if (trampoline == NULL)
    {
        subhook_remove(hook);
        subhook_free(hook);

        return -4;
    }

    *original = trampoline;

    return 0;
}
