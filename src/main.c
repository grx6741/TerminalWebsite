//------------------------------------------------------------------------------
//  debugtext-sapp.c
//  Text rendering with sokol_debugtext.h, test builtin fonts.
//------------------------------------------------------------------------------
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "sokol_debugtext.h"
#include "sokol_color.h"

#include "shaders_h/triangle.glsl.h"
#include "console.h"

static struct {
    struct {
	sg_pass_action pass_action;
	sg_attachments attachment;
	sg_attachments_desc attachment_desc;
    } offscreen;

    struct {
	sg_pass_action pass_action;
	sg_bindings bind;
	sg_pipeline pip;
    } display;

    Console console;
} state = {
    .offscreen = {
	.pass_action = {
	    .colors[0] = { .load_action=SG_LOADACTION_CLEAR, .clear_value=SG_BLACK },
	}
    },

    .display = {
	.pass_action = {
	    .colors[0].load_action=SG_LOADACTION_DONTCARE,
	    .depth.load_action=SG_LOADACTION_DONTCARE,
	    .stencil.load_action=SG_LOADACTION_DONTCARE
	}
    },

};

void create_offscreen_pass(int width, int height) {
    sg_destroy_attachments(state.offscreen.attachment);
    sg_destroy_image(state.offscreen.attachment_desc.colors[0].image);
    sg_destroy_image(state.offscreen.attachment_desc.depth_stencil.image);

    sg_image color_img = sg_make_image(&(sg_image_desc) {
	    .render_target = true,
	    .width = width,
	    .height = height,
	    .pixel_format = SG_PIXELFORMAT_RGBA8,
	    .label = "color-image"
	});

    sg_image depth_img = sg_make_image(&(sg_image_desc) {
	    .render_target = true,
	    .width = width,
	    .height = height,
	    .pixel_format = SG_PIXELFORMAT_DEPTH_STENCIL,
	    .label = "depth-image"
	});

    sg_sampler color_smp = sg_make_sampler(&(sg_sampler_desc) {
	    .wrap_u = SG_WRAP_CLAMP_TO_EDGE,
	    .wrap_v = SG_WRAP_CLAMP_TO_EDGE,
	    .min_filter = SG_FILTER_LINEAR,
	    .mag_filter = SG_FILTER_LINEAR,
	    .compare = SG_COMPAREFUNC_NEVER,
	});

    state.offscreen.attachment_desc = (sg_attachments_desc){
	    .colors[0].image = color_img,
	    .depth_stencil.image = depth_img,
	    .label = "offscreen-pass"
    };

    state.offscreen.attachment = sg_make_attachments(&state.offscreen.attachment_desc);
    state.display.bind.fs.images[SLOT__diffuse_texture] = color_img;
    state.display.bind.fs.samplers[SLOT_diffuse_texture_smp] = color_smp;
}

static void init(void) {
    // setup sokol-gfx
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });
    create_offscreen_pass(sapp_width(), sapp_height());

    float quad_vertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
    };

    sg_buffer quad_buffer = sg_make_buffer(&(sg_buffer_desc){
	    .size = sizeof(quad_vertices),
	    .data = SG_RANGE(quad_vertices),
	    .label = "quad-vertices"
	});

    state.display.bind.vertex_buffers[0] = quad_buffer;


    state.display.pip = sg_make_pipeline(&(sg_pipeline_desc) {
	    .layout = {
		.attrs = {
		    [ATTR_vs_a_pos].format = SG_VERTEXFORMAT_FLOAT2,
	    	    [ATTR_vs_a_tex_coords].format = SG_VERTEXFORMAT_FLOAT2
		}
	    },
	    .shader = sg_make_shader(display_shader_desc(sg_query_backend())),
	    .label = "display-pipeline"
	});

    state.console = console_init();
}

static void frame(void) {
    float dt = sapp_frame_duration();

    console_draw(state.console);

    // sdtx_font(CURRENT_FONT);
    // sdtx_color1i(0xFF88FF88);
    // sdtx_printf("%.*s", (int)state.console.buffer_len + 1, state.console.buffer);

    // fs_params_t fs_params;
    // fs_params.u_Resolution = (HMM_Vec2) { .X = sapp_width(), .Y = sapp_height() };

    sg_begin_pass(&(sg_pass) { 
            .action = state.offscreen.pass_action, 
            .attachments = state.offscreen.attachment 
        });
    sdtx_draw();
    sg_end_pass();

    sg_begin_pass(&(sg_pass) { 
	    .action = state.display.pass_action, 
	    .swapchain = sglue_swapchain()
	});

    sg_apply_pipeline(state.display.pip);
    sg_apply_bindings(&state.display.bind);
    // sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, &SG_RANGE(fs_params));
    sg_draw(0, 6, 1);
    sg_end_pass();

    sg_commit();
}

static void event(const sapp_event* e) {
    if (e->type == SAPP_EVENTTYPE_RESIZED) {
        create_offscreen_pass(e->framebuffer_width, e->framebuffer_height);
    }

    switch (e->type) {
	case SAPP_EVENTTYPE_RESIZED:
	    create_offscreen_pass(e->framebuffer_width, e->framebuffer_height);
	    break;
	case SAPP_EVENTTYPE_CHAR:
	    // state.console.buffer[state.console.buffer_len++] = e->char_code;
	    console_append_input_text(&state.console, e->char_code);
	    break;
	case SAPP_EVENTTYPE_KEY_DOWN:
	    switch (e->key_code) {
		// TODO: Feature Req -- Add ctrl+backspace functionality
		case SAPP_KEYCODE_BACKSPACE:
		    console_pop_input_text(&state.console);
		    break;
		default:
		    break;
	    }
	    break;
	default:
	    break;
    }
}

static void cleanup(void) {
    sdtx_shutdown();
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return (sapp_desc) {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .width = 1024,
        .height = 600,
	.event_cb = event,
        .window_title = "debugtext-sapp",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
