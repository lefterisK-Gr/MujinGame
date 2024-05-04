#version 330

struct Light {
    vec2 position;
    float radius;
};

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D texture_sampler;
const int MAX_LIGHTS = 1;
uniform Light lightPos[MAX_LIGHTS];

void main() {

    vec4 textureColor = texture(texture_sampler, fragmentUV);
    vec3 totalLightEffect = vec3(0.0);

    for(int i= 0; i < MAX_LIGHTS; i++) {
        float distance = length(fragmentPosition - lightPos[i].position);
        float attenuation = clamp(pow(100.0f / (distance + 1.0), 2.0), 0.0, 1.0);
        totalLightEffect += textureColor.rgb * attenuation;
    }

    //float distance = length(fragmentPosition - lightPos[1].position);
    //float attenuation = clamp(pow(100.0f / (distance + 1.0), 2.0), 0.0, 1.0);
    //vec3 brightenedColor = textureColor.rgb * attenuation;

    vec3 finalColor = mix(textureColor.rgb * fragmentColor.rgb, totalLightEffect, 1.0);

    color = vec4(finalColor, textureColor.a);
}