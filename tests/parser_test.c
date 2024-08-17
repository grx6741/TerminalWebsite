#include <stdio.h>
#include <stdlib.h>
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
	print_tokens_arr(tarr, buffer, buffer_len);
	printf("\n");
    }
}
