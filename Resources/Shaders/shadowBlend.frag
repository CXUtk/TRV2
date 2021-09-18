#version 330 core

in vec2 fTexCoord;
out vec4 color;

uniform sampler2D uOriginalMap;
uniform sampler2D uShadowMap;

void main() {
	vec4 baseC = texture(uOriginalMap, fTexCoord);
	vec4 maskC = texture(uShadowMap, fTexCoord);
	color = baseC * maskC;
}