#version 330

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D texture_sampler;
uniform vec2 lightPos;


void main() {

    vec4 textureColor = texture(texture_sampler, fragmentUV);

    // Calculate distance from the light source in texture space
    float distance = length(fragmentPosition - lightPos);

    // Calculate attenuation based on distance
    float attenuation = clamp(pow(100.0f / (distance + 1.0), 2.0), 0.0, 1.0);

    // Adjust texture color brightness based on attenuation
    vec3 brightenedColor = textureColor.rgb * attenuation;

    // Blend the brightened color with the original darkened texture color using vertex color as factor
    vec3 finalColor = mix(textureColor.rgb * fragmentColor.rgb, brightenedColor, attenuation);

    color = vec4(finalColor, textureColor.a);
}