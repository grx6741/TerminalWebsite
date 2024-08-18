#include "console.h"
#include "parser.h"
#include "commands.h"

#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_log.h>
#include <sokol_debugtext.h>

#include <string.h>

#define FONT_KC853 (0)
#define FONT_KC854 (1)
#define FONT_Z1013 (2)
#define FONT_CPC   (3)
#define FONT_C64   (4)
#define FONT_ORIC  (5)

#define CURRENT_FONT FONT_ORIC

#define PROMPT "grx@gowrish:> "
#define PROMPT_LEN strlen(PROMPT)


#define LOG(x) printf("[%d] %s: %d\n", __LINE__, #x, x)
// #define LOG(x)

Console console_init() {
    // setup sokol-debugtext
    sdtx_setup(&(sdtx_desc_t){
        .fonts = {
            [FONT_KC853] = sdtx_font_kc853(),
            [FONT_KC854] = sdtx_font_kc854(),
            [FONT_Z1013] = sdtx_font_z1013(),
            [FONT_CPC]   = sdtx_font_cpc(),
            [FONT_C64]   = sdtx_font_c64(),
            [FONT_ORIC]  = sdtx_font_oric()
        },
        .logger.func = slog_func,
    });

    // Fill in the tables u dumbass
    init_commands();

    return (Console) {
	.prompt = PROMPT,
	.state = CONSOLE_DONE,
	.buffer = {CURSOR},
	.buffer_len = 0,
	.std_out = (STDout) {
	    .lines_count = 1,
	    .lines[0] = (Line) {
		.buffer = PROMPT,
		.buffer_len = PROMPT_LEN,
		.is_prompt = true
	    }
	}
    };
}

void console_draw(Console console) {
    float scale_x = 1 / 2.5f;
    float scale_y = 1 / 2.5f;

    sdtx_canvas(sapp_width() * scale_x, sapp_height() * scale_y);

    sdtx_origin(0.4f, 0.4f);
    // sdtx_home();
    sdtx_font(CURRENT_FONT);
    sdtx_color1i(0xFF00FF00);
    for (int i = 0; i < console.std_out.lines_count; i++) {
	Line line = console.std_out.lines[i];
	sdtx_puts(line.buffer);

	if (!line.is_prompt) sdtx_crlf();
    }
    // sdtx_puts(console.prompt);
    // sdtx_crlf();
    
    // +1 for including the cursor as well
    sdtx_printf("%.*s", (int)console.buffer_len + 1, console.buffer);
}

void console_read_buffer(Console* console) {

    Line* last_line = &console->std_out.lines[console->std_out.lines_count];
    strncpy(last_line->buffer + last_line->buffer_len, console->buffer, console->buffer_len);

    TokenArray tarr = get_all_tokens(console->buffer, console->buffer_len);
    console->std_out.lines_count++;

    // Token program_tok = tarr.tokens[0];
    // size_t program_tok_size = program_tok.end - program_tok.start + 1;
    // char program_name[program_tok_size];
    // strncpy(program_name, console->buffer + program_tok.start, program_tok_size);

    // Token program_name = tarr.tokens[0];
    // String program_str = tokenToString(console->buffer, program_name);

    // console_print(console, text_format("\n"));

    // console_print(console, text_format("Program: %s", program_str.buffer));

    // for (int i = 1; i < tarr.tokens_count; i++) {
    //     Token arg_name = tarr.tokens[i];
    //     String arg_str = tokenToString(console->buffer, arg_name);
    //     console_print(console, text_format("Arguement %d: %s", i, arg_str.buffer));
    // }

    // console_print(console, text_format("\n"));

    // Match program name with all available commands
    console_print(console, text_format("\n"));

    execute_command(console, tarr);

    console_print(console, text_format("\n"));

    console->std_out.lines[console->std_out.lines_count++] = (Line) {
	.buffer = PROMPT,
	.buffer_len = PROMPT_LEN,
	.is_prompt = true
    };

    // Clear the input buffer
    console->buffer_len = 0;
    // Reset the cursor
    console->buffer[console->buffer_len] = CURSOR;
}

void console_append_input_text(Console* console, char input_char) {
    if (console->buffer_len < MAX_BUFFER_LEN) {
	console->buffer[console->buffer_len++] = input_char;
	console->buffer[console->buffer_len] = CURSOR;
    }
}


void console_pop_input_text(Console* console) {
    if (console->buffer_len > 0) {
	console->buffer_len--;
	console->buffer[console->buffer_len] = CURSOR;
    }
}


void console_print(Console* console, String str) {
    if (console->std_out.lines_count >= MAX_LINES_COUNT) return;
    if (str.buffer_len >= MAX_LINE_BUFFER_LEN) return;

    size_t curr_line = console->std_out.lines_count;
    strncpy(console->std_out.lines[curr_line].buffer, str.buffer, str.buffer_len);

    console->std_out.lines_count++;
}
