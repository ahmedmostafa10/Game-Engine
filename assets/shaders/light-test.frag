#version 330 core

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    int type; // 0 = directional, 1 = point, 2 = spot
};

#define MAX_LIGHTS 8

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 FragColor;

uniform vec3 camera_position;
uniform Light lights[MAX_LIGHTS];
uniform int light_count;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D emissionMap;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(camera_position - fragPos);
    vec3 albedo = texture(albedoMap, fragTexCoord).rgb;
    float specularStrength = texture(specularMap, fragTexCoord).r;
    float roughness = texture(roughnessMap, fragTexCoord).r;
    float ao = texture(aoMap, fragTexCoord).r;
    vec3 emission = texture(emissionMap, fragTexCoord).rgb;

    vec3 finalColor = vec3(0.0);

    for(int i = 0; i < light_count; ++i) {
        vec3 L;
        if(lights[i].type == 0) { // DIRECTIONAL
            L = normalize(-lights[i].direction);
        } else {
            L = normalize(lights[i].position - fragPos);
        }

        vec3 H = normalize(V + L);
        float diff = max(dot(N, L), 0.0);
        float spec = pow(max(dot(N, H), 0.0), mix(8.0, 1.0, roughness)) * specularStrength;

        vec3 lightColor = lights[i].color;

        finalColor += (albedo * diff + spec) * lightColor;
    }

    // Add ambient occlusion and emission
    finalColor = mix(vec3(0.03) * albedo, finalColor, ao) + emission;

    FragColor = vec4(finalColor, 1.0);
}
