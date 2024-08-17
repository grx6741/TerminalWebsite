#include "console.h"
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

    return (Console) {
	.prompt = PROMPT,
	.state = CONSOLE_DONE,
	.buffer = {CURSOR},
	.buffer_len = 0,
	.stdout = (STDout) {
	    .lines_count = 1,
	    .lines[0] = (Line) {
		.buffer = PROMPT,
		.buffer_len = PROMPT_LEN
	    }
	}
    };
}

void console_draw(Console console) {
    float scale_x = 1 / 2.5f;
    float scale_y = 1 / 2.5f;

    sdtx_canvas(sapp_width() * scale_x, sapp_height() * scale_y);

    sdtx_origin(0.4f, 0.4f);
    sdtx_home();
    sdtx_font(CURRENT_FONT);
    sdtx_color1i(0xFF00FF00);
    sdtx_puts(console.prompt);
    
    // +1 for including the cursor as well
    sdtx_printf("%.*s", (int)console.buffer_len + 1, console.buffer);
}

Command console_read_buffer(Console* console) {
    return (Command)0;
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
