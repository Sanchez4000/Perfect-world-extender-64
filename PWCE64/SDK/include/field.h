#ifndef PWCE_FIELD_H
#define PWCE_FIELD_H

#include "types.h"

static inline void* field_at(void* object, uint32 offset)
{
    return (uint8*)object + offset;
}

#endif
