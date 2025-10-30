#version 410 core

layout(location=0) in vec3 in_Position;

// 전역변수 처럼 uniform 변수로 4x4 행렬(mat4)을 선언합
uniform mat4 uTransform;

void main(void)
{   //in_Position에  1을 붙여 homogeneous coordinate를 만들고, 1에서 선언한 transformation matrix를 곱하여
    //변환이 적용된 3차원 homogeneous coordinate를 계산
    gl_Position = uTransform * vec4(in_Position, 1.0); //이 homogeneous coordinate를 최종 좌표(gl_Position)로
}
