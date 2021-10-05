#version 330 core

in vec2 fTexCoord;
out vec4 color;

uniform float dx = 0.01;

float genRand(vec2 coord)
{
	return fract(sin(dot(coord, vec2(12.9898, 78.233)) * 43758.5453));
}

vec2 getDir(vec2 coord)
{
	float r = genRand(coord) * 2 * 3.1415926;
	return vec2(cos(r), sin(r));
}

float lerp2(float x)
{
	float x3 = x * x * x;
	return ((6 * x - 15) * x + 10) * x3; 
}

float noise(vec2 coord)
{
	vec2 v_frac = fract(coord);
	vec2 v_floor = floor(coord);

	float d1 = dot(getDir(v_floor), v_frac);
	float d2 = dot(getDir(v_floor + vec2(1.0, 0.0)), v_frac - vec2(1.0, 0.0));
	float d3 = dot(getDir(v_floor + vec2(0.0, 1.0)), v_frac - vec2(0.0, 1.0));
	float d4 = dot(getDir(v_floor + vec2(1.0, 1.0)), v_frac - vec2(1.0, 1.0));

	v_frac.x = lerp2(v_frac.x);
	v_frac.y = lerp2(v_frac.y);
	float v1 = mix(d1, d2, v_frac.x);
	float v2 = mix(d3, d4, v_frac.x);
	return mix(v1, v2, v_frac.y);
}

float fBm(vec2 coord, int K)
{
	float total = 0.0;
	float multiple = 1.0;
	for(int i = 0; i < K; i++)
	{
		total += 1.0 / multiple * noise(coord * multiple);
		multiple *= 2.0;
	}
	return total;
}


void main() {
	vec2 coord = fTexCoord * 0.1;
	float v = fBm(coord, 1) * 0.5 + 0.5;

	float vL = fBm(coord - vec2(dx, 0), 4);
	float vR = fBm(coord + vec2(dx, 0), 4);
	float vB = fBm(coord - vec2(0, dx), 4);
	float vT = fBm(coord + vec2(0, dx), 4);

	vec3 dpdx = normalize(vec3(1, 0, (vR - vL) / (2 * dx)));
	vec3 dpdy = normalize(vec3(0, 1, (vT - vB) / (2 * dx)));

	vec3 N = normalize(cross(dpdx, dpdy));
	float k = dot(N, vec3(0, 0, 1));
	color = vec4(N * 0.5 + 0.5, 1.0);
}