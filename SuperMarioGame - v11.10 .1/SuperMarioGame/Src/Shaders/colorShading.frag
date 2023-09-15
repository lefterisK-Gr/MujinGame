#version 330

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color; //rgb value

uniform float time;

void main() {
	float rValue = fragmentColor.r * (cos(fragmentPosition.x * 4.0 + time) + 1.0) * 0.5;
	float gValue = fragmentColor.g * (cos(fragmentPosition.y * 16.0 + time) + 1.0) * 0.5;
	float bValue = fragmentColor.b * (sin(fragmentPosition.x * 2.0 + time) + 1.0) * 0.5;
	color = vec4(rValue, gValue, bValue, fragmentColor.a);
}