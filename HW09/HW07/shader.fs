#version 410 core

in vec3 normal; // 1. 전달된 normal을 받기 위해 in 변수를 전역변수로 선언
out vec4 out_Color;

void main(void)
{
    //2.out_Color 값에는 이 normal에 1(투명도 값)을 붙여서 vec4를 만들어 전달
    out_Color = vec4(normal, 1.0);
}
