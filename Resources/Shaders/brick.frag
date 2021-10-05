#version 330 core

in vec2 fTexCoord;
out vec4 color;

uniform float brick_height = 0.85;
uniform float brick_width = 2.25;
uniform float groove_size = 0.05;
uniform float offset[2] = { 0.0, 0.25 };

const vec4 brick_color = vec4( 0.4, 0.2, 0.1, 1 );
const vec4 groove_color = vec4( 0.5, 0.6, 0.5, 1 );

float genRand(vec2 coord)
{
	return fract(sin(dot(coord, vec2(12.9898, 78.233)) * 43758.5453));
}


void main() {
	int y = int(floor(fTexCoord.y / brick_height));
	vec2 coord = fTexCoord;

	if (y % 2 == 0) 
	{
		coord += vec2(1.0, 0);
	}
	int x = int(floor(coord / brick_width));
	vec2 subCoord = mod(coord, vec2(brick_width, brick_height));

	if(subCoord.x >= groove_size && subCoord.x <= brick_width - groove_size && subCoord.y >= groove_size && subCoord.y <= brick_height - groove_size)
	{
		color = brick_color * (0.5 + 0.5 * genRand(vec2(x, y)));
	}
	else
	{
		color = groove_color;	
	}
}