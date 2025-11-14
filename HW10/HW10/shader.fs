#version 410 core

in vec3 normal; //  전달된 normal을 받기 위해 in 변수를 전역변수로 선언
in vec3 worldPos; //2-2.다음 vertex shader로 부터 interpolation된 점들의 3차원 좌표를 얻기 위해 in변수로 vec3 worldPos를 선언
out vec4 out_Color;

//2-1.먼저 몇 가지 uniform 변수를 만듭니다.
uniform vec3 diffColor  = vec3(1.0, 0.5, 0.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos   = vec3(10.0, 20.0, 10.0);

void main(void)
{
    //2-3.main 함수 속에서, light 방향 (보통 L이라는 이름으로 함)을 계산. lightPos에서 worldPos를 빼고 normalize 하면 됩니다.
    vec3 toLight = lightPos - worldPos;
    vec3 L = normalize(toLight);
    //2-4. 길이를 1로 만들기 위해 normalize 하여 다른 변수(보통 N이라고 합니다.)에 저장.
    vec3 N = normalize(normal); //추가함

    //2-5.diffColor에 N·L을 곱하고 lightColor를 곱합니다
    vec3 color = diffColor * (max(0.0, dot(N, L))) * lightColor;

    //gamma correction
    // color에 1(불투명도)을 붙여 out_Color 에 적용합니다.
    vec3 temp = pow( color, vec3( 1.0 / 2.2 ) );
    out_Color = vec4(temp, 1.0 );
}
