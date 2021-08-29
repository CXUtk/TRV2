#version 330 core
/*
*   ���ڻ���2D Sprite��Shader
*   ��ɲ��֣�vec2 pos, vec2 texCoord
*/
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoord;

uniform mat4 uTransform;

out vec2 vTexCoord;

void main(){
    gl_Position = uTransform * vec4(vertex, 0.0, 1.0);
    vTexCoord = texCoord;
}

