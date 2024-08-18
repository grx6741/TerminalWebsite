#pragma once

#include "parser.h"
#include "console.h"

typedef enum {
    COMMAND_INVALID,
    COMMAND_WHO_AM_I,
    COMMAND_HELP,
    COMMANDS_COUNT
} Command;

// To avoid circular dependency
typedef void (*command_t)(Console* console, TokenArray tarr);

static const char* _valid_commands[COMMANDS_COUNT];
static command_t _valid_commands_funcs[COMMANDS_COUNT];

// To fill in the _valid_commands and _valid_commands_funcs table
void init_commands();
Command match_program_name(String program_name);
void execute_command(Console* console, TokenArray tarr);
