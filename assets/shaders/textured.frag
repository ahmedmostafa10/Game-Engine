#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
uniform sampler2D tex;

void main(){
    // by multiplying the tint with the vertex color and with the texture color 
    vec4 texture_color = texture(tex, fs_in.tex_coord);
    frag_color = tint * fs_in.color * texture_color;
}