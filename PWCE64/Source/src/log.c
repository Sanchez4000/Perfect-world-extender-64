#include "log.h"

#include <stdio.h>

#define LOG_PREFIX "[pwce] "

static FILE* file = NULL;

int32 pwce_log_open(const char* path)
{
    file = fopen(path, "w");

    if (file == NULL)
        return -1;

    return 0;
}

void pwce_log(const char* message)
{
    if (file == NULL || message == NULL)
        return;

    fprintf(file, LOG_PREFIX "%s\n", message);
    fflush(file);
}
