#version 330

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;
uniform float time;

uniform sampler2D texture_sampler;

void main() {
    float waveAmplitude = 0.003; // Amplitude of the wave
    float waveFrequency = 100.0; // Frequency of the wave

     vec2 wave = vec2(
        waveAmplitude * sin(fragmentUV.y * waveFrequency + time), 
        waveAmplitude * sin(fragmentUV.x * waveFrequency + time)
    );

    vec2 waveUV = fragmentUV + wave;
    vec4 textureColor = texture(texture_sampler, waveUV);

    color = fragmentColor * 0.6 * textureColor;
}