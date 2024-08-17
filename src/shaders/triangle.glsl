@ctype vec2 HMM_Vec2
@ctype vec3 HMM_Vec3
@ctype mat4 HMM_Mat4

@vs vs
in vec2 a_pos;
in vec2 a_tex_coords;
out vec2 tex_coords;

void main() {
    gl_Position = vec4(a_pos, 0.0, 1.0);
    tex_coords = a_tex_coords;
}
@end

@fs fs

in vec2 tex_coords;
out vec4 frag_color;

uniform texture2D _diffuse_texture;
uniform sampler diffuse_texture_smp;
#define diffuse_texture sampler2D(_diffuse_texture, diffuse_texture_smp)

void main() {
    frag_color = texture(diffuse_texture, tex_coords);
}
@end

@program display vs fs
