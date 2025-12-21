// 컴퓨터 그래픽스 과제 38 shadow.vs
#version 410 core

layout(location = 0) in vec3 in_Position;
uniform mat4 shadowMVP;

void main()
{
    gl_Position = shadowMVP * vec4(in_Position, 1.0);
}                                         
