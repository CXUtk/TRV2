#version 330 core

in vec2 fTexCoord;
out vec4 color;

uniform sampler2D uOriginalMap;
uniform sampler2D uShadowMap;

void main() {
	vec4 baseC = texture(uOriginalMap, fTexCoord);
	vec4 maskC = texture(uShadowMap, fTexCoord);
	vec4 tmp = baseC * maskC;

	// Gamma Correction
	tmp.r = pow(tmp.r, 1 / 2.2);
	tmp.g = pow(tmp.g, 1 / 2.2);
	tmp.b = pow(tmp.b, 1 / 2.2);

	color = tmp;
}