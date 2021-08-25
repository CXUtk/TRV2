#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/

in vec2 vTexCoord;

out vec4 color;

uniform sampler2D uTexture;
uniform bool uHorizontal;

float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
//float weight[5] = float[] (0.2, 0.2, 0.2, 0.2, 0.2);

#define NUM_SAMPLES 100
#define DX 1.0 / NUM_SAMPLES
#define SIGMA 0.2
#define PI 3.141592653589793
#define PI2 6.283185307179586



float sqr(float x){
    return x * x;
}

float gauss1d(float offset, float sigma) {
    float top = exp(-sqr(offset) / (2 * sqr(sigma)));
    float bot = sigma * sqrt(PI2);
    return top / bot;
}

float gauss2d(vec2 offset, float sigma) {
    float top = exp(-0.5 * (sqr(offset.x / sigma) + sqr(offset.y / sigma)));
    float bot = 2.0 * sqr(sigma) * PI;
    return top / bot;
}


float decode(vec4 c){
    return dot(c, 
        vec4(256.0, 1.0, 1.0 / 256.0, 1.0 / 65536.0));
}

vec4 encode(float v){
    vec4 bitshift = vec4(1.0 / 256.0, 1.0, 256.0, 65536.0);
    vec4 tmp = fract(v * bitshift);
    return vec4( tmp.xyz - tmp.yzw / 256.0, tmp.w ) + 1.0 / 512.0;
}




//    float strength = 0.0;
////    for(int i = 0; i < NUM_SAMPLES; i++) {
////        vec2 offset = poissonDisk[i] * 0.05;
////        vec4 c = texture(uScreenTexture, vTexCoord + offset);
////        strength += dot(c, vec4(256.0, 1.0, 1.0 / 256.0, 1.0 / 256.0 / 256.0)) * gauss(offset, 0.5);
////    }
//    for(int i = -10; i <= 10; i++){
//        for(int j = -10; j <= 10; j++){
//            vec2 offset = vec2(i, j) * 0.003;
//            vec4 c = texture(uScreenTexture, vTexCoord + offset);
//            strength += dot(c, vec4(256.0, 1.0, 1.0 / 256.0, 1.0 / 256.0 / 256.0)) * gauss(offset * 10, 0.1);
//        }
//    }
//    strength /= NUM_SAMPLES;

//    vec3 rColor = uColor * original;
//    rColor.x = pow(rColor.x, 1 / 2.2);
//    rColor.y = pow(rColor.y, 1 / 2.2);
//    rColor.z = pow(rColor.z, 1 / 2.2);

void main() {
    // color = texture(uScreenTexture, vTexCoord);
    vec2 tex_offset = 1.0 / textureSize(uTexture, 0);

    float origin = decode( texture(uTexture, vTexCoord) );

    float totWeight = 0;

    float result = origin * gauss2d(vec2(0, 0), SIGMA);
     for(int i = -NUM_SAMPLES; i <= NUM_SAMPLES; ++i){
        for(int j = -NUM_SAMPLES; j <= NUM_SAMPLES; ++j) {
            float w = gauss2d(vec2(i, j) * 0.01, SIGMA);
            result += decode(texture(uTexture, vTexCoord + vec2(tex_offset.x * i, tex_offset.y * j))) * w;
        }
    }
    result /= NUM_SAMPLES * NUM_SAMPLES;
    color = encode(max(result, origin));
}