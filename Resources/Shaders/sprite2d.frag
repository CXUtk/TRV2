#version 330 core

in vec2 fTexCoord;
in vec4 fColor;
in float fTexSlot;

out vec4 color;

uniform sampler2D uTextures[32];

void main() {
	color = texture(uTextures[int(fTexSlot)], fTexCoord) * fColor;
}