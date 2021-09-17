#version 330 core

in vec2 fTexCoord;
out vec4 color;

uniform sampler2D uOriginalMap;
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(uOriginalMap, 0); // gets size of single texel

    float w = 1.0 / 41.0;

    vec3 origin = texture(uOriginalMap, fTexCoord).rgb;
    vec3 result = origin * w * 4.0; // current fragment's contribution

    if(horizontal)
    {
        for(int i = 1; i < 20; ++i)
        {
            result += texture(uOriginalMap, fTexCoord + vec2(tex_offset.x * i, 0.0)).rgb * w;
            result += texture(uOriginalMap, fTexCoord - vec2(tex_offset.x * i, 0.0)).rgb * w;
        }
    }
    else
    {
        for(int i = 1; i < 20; ++i)
        {
            result += texture(uOriginalMap, fTexCoord + vec2(0.0, tex_offset.y * i)).rgb * w;
            result += texture(uOriginalMap, fTexCoord - vec2(0.0, tex_offset.y * i)).rgb * w;
        }
    }
    color = vec4(result, 1.0);
}