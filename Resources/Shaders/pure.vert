#version 330 core
layout (location = 0) in vec2 vPos;
layout (location = 1) in vec4 vColor;

uniform mat4 uWorldTransform;

out vec4 fColor;

void main(){
    gl_Position = uWorldTransform * vec4(vPos, 0.0, 1.0);
    fColor = vColor;
}

