#include "log.h"

#include <string.h>
#include <unistd.h>

#define LOG_PREFIX "[pwse] "

void pwse_log(const char* message)
{
    if (message == NULL)
        return;

    (void)write(1, LOG_PREFIX, sizeof(LOG_PREFIX) - 1);
    (void)write(1, message, strlen(message));
    (void)write(1, "\n", 1);
}
