#pragma once

#include "parser.h"

typedef enum {
    COM_INVALID,
    COM_WHO_AM_I,
    COM_LS,
    COMMANDS_COUNT
} Command;

// To avoid circular dependency
//			  Console*
typedef void (*command_t)(void* console, TokenArray tarr);

static const char* _valid_commands[COMMANDS_COUNT];
static command_t _valid_commands_funcs[COMMANDS_COUNT];

// To fill in the _valid_commands and _valid_commands_funcs table
void init_commands();
