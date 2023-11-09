#version 330

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color; //rgb value

uniform float time;
uniform sampler2D texture_sampler;


void main() {

    vec4 textureColor = texture(texture_sampler, fragmentUV);

    float rValue = fragmentColor.r * (cos(fragmentPosition.x * 4.0 + time) + 1.0) * 0.5;
    float gValue = fragmentColor.g * (cos(fragmentPosition.y * 16.0 + time) + 1.0) * 0.5;
    float bValue = fragmentColor.b * (sin(fragmentPosition.x * 2.0 + time) + 1.0) * 0.5;
    color = vec4(rValue, gValue, bValue, fragmentColor.a) * textureColor;
}