#version 330 core
/*
*   ���ڻ���2D Sprite��Shader
*   ��ɲ��֣�vec2 pos, vec2 texCoord
*/
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 vColor;

uniform mat4 uWorldTransform;

out vec2 fTexCoord;
out vec4 fColor;

void main(){
    gl_Position = uWorldTransform * vec4(vertex, 0.0, 1.0);
    fTexCoord = texCoord;
    fColor = vColor;
}

