#version 330 core
in vec2 vTexCoord;

out vec4 color;

uniform vec4 uColor;

void main() {
	color = uColor;
}