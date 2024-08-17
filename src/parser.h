#pragma once

#include <stdint.h>
#include <stdlib.h>

#define MAX_TOKENS 10

typedef struct {
    uint32_t start; // where the token starts in the cmd buffer
    uint32_t end; // where it ends
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    size_t tokens_count;
} TokenArray;

void add_token(TokenArray* tarr, Token token);
void print_tokens_arr(TokenArray tarr, char* buffer, size_t buffer_len);
TokenArray get_all_tokens(char* buffer, size_t buffer_len);

// returns of next non white space character's index in a buffer
uint32_t _next_nonwp(char* buffer, size_t buffer_len, uint32_t start);

// returns of next white space character's index in a buffer
uint32_t _next_wp(char* buffer, size_t buffer_len, uint32_t start);
