#version 330 core

in vec2 fTexCoord;
out vec4 color;

uniform sampler2D uPrevMap;

uniform float uBlendFactor;

void main() {
	color = texture(uPrevMap, fTexCoord);
}