#include "commands.h"
#include "console.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// All valid commands' funciton pointers

void command_who_am_i(Console* console, TokenArray tarr) {

    char* message[2] = {
        "Welcome to Gowrish's computer\n",
        "Try 'help' command for a list of commands",
    };

    for (int i = 0; i < 2; i++) {
        console_print(console, (String) { 
        	.buffer = message[i], 
        	.buffer_len = strlen(message[i])
            });
    }
}

void command_help(Console* console, TokenArray tarr) {
    char* message = "Here are some commands you can try";

    console_print(console, (String) { 
	    .buffer = message, 
	    .buffer_len = strlen(message)
	    });

    console_print(console, text_format("\n"));

    for (int i = 1; i < COMMANDS_COUNT; i++) {
        const char* command_name = _valid_commands[i];
        console_print(console, text_format("%d: %s", i, command_name));
    }
}

void init_commands() {
    // Table for command as string
    _valid_commands[COMMAND_WHO_AM_I] = "whoami";
    _valid_commands[COMMAND_HELP] = "help";

    // Table for command as function ptr

    _valid_commands_funcs[COMMAND_WHO_AM_I] = command_who_am_i;
    _valid_commands_funcs[COMMAND_HELP] = command_help;
}

Command match_program_name(String program_name) {
    for (int i = 1; i < COMMANDS_COUNT; i++) {
	if (strncmp(program_name.buffer, _valid_commands[i], program_name.buffer_len) == 0) {
	    return i;
	}
    }
    return COMMAND_INVALID;
}

void execute_command(Console* console, TokenArray tarr) {
    Token program_name = tarr.tokens[0];
    String program_str = tokenToString(console->buffer, program_name);

    Command command = match_program_name(program_str);

    if (command == COMMAND_INVALID) {
        console_print(console, text_format("Invalid Command: %s", program_str.buffer));
    } else if (COMMAND_INVALID < command && command < COMMANDS_COUNT) {
        _valid_commands_funcs[command](console, tarr);
    }

    free((void*)program_str.buffer);
}
