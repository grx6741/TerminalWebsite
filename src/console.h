#pragma once

#include "commands.h"
#include "stdout.h"

#define CURSOR ('}' + 2)

typedef enum {
    CONSOLE_LOAD = 0,
    CONSOLE_RUNNING,
    CONSOLE_DONE
} ConsoleState ;

typedef struct {
    const char* prompt; // "grx@gowrish:>"
    ConsoleState state;
    char buffer[MAX_BUFFER_LEN + 1]; // 1 extra for cursor
    size_t buffer_len;

    STDout std_out;
} Console;

Console console_init();
void console_read_buffer(Console* console);
void console_draw(Console console);

void console_print(Console* console, String str);

void console_append_input_text(Console* console, char input_char);
void console_pop_input_text(Console* console);
