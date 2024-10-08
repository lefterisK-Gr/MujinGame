#version 330

in vec4 fragmentColor;
in vec2 fragmentUV;

uniform float time;

out vec4 color; //rgb value

void main() {

    float waveX = sin(fragmentUV.x * 5.0 + time * 0.5) * cos(fragmentUV.y * 2.0 + time * 0.3);
    float waveY = cos(fragmentUV.y * 5.0 + time * 0.5) * pow(cos(fragmentUV.x + time * 0.1),2);
    float wave = (waveX + waveY) / 2.0; // Combine waves

    wave = (wave + 1.0) / 2.0;

    float alpha = mix(0.0, 0.4, wave);

    color = vec4(fragmentColor.rgb, alpha);
}