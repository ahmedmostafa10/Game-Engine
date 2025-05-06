#version 330 core

#define MAX_LIGHTS 8

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    int type; // 0 = DIRECTIONAL, 1 = POINT, 2 = SPOT
    float innerConeCos;
    float outerConeCos;
};

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 FragColor;

uniform vec3 camera_position;
uniform Light lights[8];
uniform int light_count;

// Texture maps
uniform sampler2D albedoMap;
uniform sampler2D specularMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D emissionMap;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(camera_position - fragPos);

    vec4 albedoSample = texture(albedoMap, fragTexCoord);
    vec3 albedo = albedoSample.rgb;
    float alpha = albedoSample.a;

    float specularStrength = texture(specularMap, fragTexCoord).r;
    float roughness = texture(roughnessMap, fragTexCoord).r;
    float ao = texture(aoMap, fragTexCoord).r;
    vec3 emission = texture(emissionMap, fragTexCoord).rgb;

    vec3 finalColor = vec3(0.0);

    for(int i = 0; i < light_count; ++i) {
        Light light = lights[i];
        vec3 L;

        if(light.type == 0) { // Directional
            L = normalize(-light.direction);
        } else if(light.type == 1 || light.type == 2) { // Point or Spot
            L = normalize(light.position - fragPos);
        }

        // Spotlight attenuation
        float spotFactor = 1.0;
        if(light.type == 2) {
            float cosAngle = dot(-L, normalize(light.direction));
            if(cosAngle < light.outerConeCos) {
                spotFactor = 0.0;
            } else if(cosAngle < light.innerConeCos) {
                float t = (cosAngle - light.outerConeCos) / (light.innerConeCos - light.outerConeCos);
                spotFactor = clamp(t, 0.0, 1.0);
            }
        }

        float diff = max(dot(N, L), 0.0);

        vec3 H = normalize(V + L);
        float spec = pow(max(dot(N, H), 0.0), mix(8.0, 1.0, roughness)) * specularStrength;

        vec3 lightColor = light.color * spotFactor;
        finalColor += (albedo * diff + spec) * lightColor;
    }

    // Ambient occlusion + emission
    finalColor = mix(vec3(0.03) * albedo, finalColor, ao) + emission;

    FragColor = vec4(finalColor, alpha);
}

