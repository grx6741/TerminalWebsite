#pragma once

#include <stddef.h>

typedef struct {
    const char* buffer;
    size_t buffer_len; // Including the Null termination Character
} String;

String text_format(const char* fmt, ...);
