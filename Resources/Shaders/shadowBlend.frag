#version 330 core

in vec2 fTexCoord;
out vec4 color;

uniform sampler2D uOriginalMap;
uniform sampler2D uShadowMap;

void main() {
	vec4 baseC = texture(uOriginalMap, fTexCoord);
	vec4 maskC = texture(uShadowMap, fTexCoord);

	// Gamma Correction
	maskC.r = pow(maskC.r, 1 / 2.2);
	maskC.g = pow(maskC.g, 1 / 2.2);
	maskC.b = pow(maskC.b, 1 / 2.2);

	color = baseC * maskC;
}