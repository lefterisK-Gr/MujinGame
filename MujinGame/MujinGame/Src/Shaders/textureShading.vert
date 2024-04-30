#version 330

in vec2 vertexPosition; //vec2 is array of 2 floats
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertexPosition.xy, 0.0, 1.0);

    fragmentPosition = vertexPosition;

    fragmentColor = vec4(vertexColor.xyz * gl_Position.x, 1.0f);

    fragmentUV = vertexUV;
}