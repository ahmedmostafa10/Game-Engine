#version 330 core

in Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} fs_in;

out vec4 frag_color;

uniform sampler2D tex;

void main(){
    // Sample the texture at the received texture coordinates
    frag_color = texture(tex, fs_in.tex_coord);
}