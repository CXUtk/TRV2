#version 330 core

in vec2 fTexCoord;
out vec4 color;

void main() {
	color = vec4(fTexCoord, 0, 1);
}