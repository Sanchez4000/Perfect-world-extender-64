#ifndef PWCE_LOG_H
#define PWCE_LOG_H

#include "types.h"

int32 pwce_log_open(const char* path);
void pwce_log(const char* message);

#endif
