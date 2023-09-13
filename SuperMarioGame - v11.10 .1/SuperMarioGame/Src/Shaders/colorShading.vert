#version 330

in vec2 vertexPosition; //vec2 is array of 2 floats

void main() {
	gl_Position.xy = vertexPosition;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0; //indicate that the coordinates are normalized
}