#pragma once

#include <stddef.h>
#include <stdbool.h>

#define MAX_BUFFER_LEN 30
#define MAX_LINE_BUFFER_LEN 50
#define MAX_LINES_COUNT 1024

typedef struct {
    char buffer[MAX_LINE_BUFFER_LEN]; // 1 extra for cursor
    size_t buffer_len;
    bool is_prompt;
} Line; // every line in stdout

typedef struct {
    Line lines[MAX_LINES_COUNT];
    size_t lines_count;
} STDout;
