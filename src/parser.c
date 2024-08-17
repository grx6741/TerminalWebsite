#include "parser.h"
#include <stdio.h>
#include <ctype.h>

// #define LOG(x) printf("[%d] %s: %d\n", __LINE__, #x, x)
#define LOG(x)

uint32_t _next_nonwp(char* buffer, size_t buffer_len, uint32_t start) {
    // TODO: check if buffer is all white space
    for (uint32_t i = start; i < buffer_len; i++) {
	if (!isspace(buffer[i])) return i;
    }
    return buffer_len;
}

uint32_t _next_wp(char* buffer, size_t buffer_len, uint32_t start) {
    // TODO: check if buffer is all white space
    for (uint32_t i = start; i < buffer_len; i++) {
	if (isspace(buffer[i])) return i;
    }
    return buffer_len;
}

void add_token(TokenArray* tarr, Token token) {
    if (tarr->tokens_count < MAX_TOKENS) {
	tarr->tokens[tarr->tokens_count++] = token;
    }
}

void print_tokens_arr(TokenArray tarr, char* buffer, size_t buffer_len) {
    printf("Buffer: %s -- TokensCount: %zu\n", buffer, tarr.tokens_count);
    for (int i = 0; i < tarr.tokens_count && i < MAX_TOKENS; i++) {
        Token tok = tarr.tokens[i];
        int len = tok.end - tok.start + 1;
        printf("[TOKEN: %d] %.*s -- [%d-%d]\n", i, len, buffer + tok.start, tok.start, tok.end);;
    }
}

TokenArray get_all_tokens(char* buffer, size_t buffer_len) {
    TokenArray tarr = {0};

    uint32_t i = 0;
    while (i < buffer_len) {
	LOG(i);
	uint32_t start = _next_nonwp(buffer, buffer_len, i); // first char's index
	LOG(start);
	if (start == buffer_len) break;
	uint32_t end = _next_wp(buffer, buffer_len, start) - 1; // last char's index
	LOG(end);

	add_token(&tarr, (Token) {
		.start = start,
		.end = end,
	    });

	i = end + 1;
    }

    return tarr;
}
