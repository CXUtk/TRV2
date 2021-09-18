#version 330 core

in vec2 fTexCoord;
out vec4 color;

void main() {
	vec2 diff = fTexCoord - vec2(0.5, 0.5);
	float len = length(diff);
	float x = 1.0 / (1.0 + 16 * len * len);
	color = vec4(x, x, x, x);
}