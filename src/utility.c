#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

String text_format(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    size_t buffer_len = vsnprintf(NULL, 0, fmt, args);
    char* buffer = malloc(sizeof(char) * (buffer_len + 1));

    vsnprintf(buffer, buffer_len + 1, fmt, args);
    buffer[buffer_len] = '\0';

    va_end(args);

    return (String) {
	.buffer = buffer,
	.buffer_len = buffer_len + 1
    };
}
