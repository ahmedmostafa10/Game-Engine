#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 transform;
uniform mat4 model;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
    vec4 worldPosition = model * vec4(position, 1.0);
    fragPos = worldPosition.xyz;
    fragNormal = mat3(transpose(inverse(model))) * normal;
    fragTexCoord = texCoord;

    gl_Position = transform * vec4(position, 1.0);
}