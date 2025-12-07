// 컴퓨터 그래픽스 과제 35
#version 410 core

layout(location = 0) out vec3 fragmentdepth;

void main()
{
    fragmentdepth = vec3(gl_FragCoord.z);
}                                             
