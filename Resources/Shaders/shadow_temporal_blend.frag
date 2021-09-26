#version 330 core

in vec2 fTexCoord;
in vec4 fColor;
in float fTexSlot;

out vec4 color;

uniform sampler2D uTextures[32];

void main() {
	vec3 c = texture(uTextures[int(fTexSlot)], fTexCoord).rgb;
	c.r = floor(min(c.r * fColor.a, 0.9999) * 256) / 256.0;
	c.g = floor(min(c.g * fColor.a, 0.9999) * 256) / 256.0;
	c.b = floor(min(c.b * fColor.a, 0.9999) * 256) / 256.0;
	color = vec4(c, 1.0);
}