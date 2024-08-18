#include <stdio.h>
#include <string.h>

#include "parser.h"

int main() {
    char* buffers[] = {
	"    cd",
	"cd Desktop",
	"   cd     ",
	"\n",
	"\tcd\t\ta",
	"$a $b",
    };
    size_t buffers_count = 6;

    for (int i = 0; i < buffers_count; i++) {
	char* buffer = buffers[i];
	size_t buffer_len = strlen(buffer);
	TokenArray tarr = get_all_tokens(buffer, buffer_len);
	printf("Test[%d] : Buffer = '%s'\n", i, buffer);
	for (int j = 0; j < tarr.tokens_count; j++) {
	    String token_str = tokenToString(buffer, tarr.tokens[j]);
	    printf("Token[%d] : %s\n", j, token_str.buffer);
	}
	// print_tokens_arr(tarr, buffer, buffer_len);
	printf("\n");
    }
}
