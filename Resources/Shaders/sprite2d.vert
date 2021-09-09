#version 330 core
/*
*   用于绘制2D Sprite的Shader
*   组成部分：vec2 pos, vec2 texCoord
*/
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec4 vColor;
layout (location = 3) in float vTexSlot;

uniform mat4 uWorldTransform;

out vec2 fTexCoord;
out vec4 fColor;
out float fTexSlot;

void main(){
    gl_Position = uWorldTransform * vec4(vertex, 0.0, 1.0);

    fTexCoord = vTexCoord;
    fColor = vColor;
    fTexSlot = vTexSlot;
}

