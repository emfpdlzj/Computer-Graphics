// 컴퓨터 그래픽스 과제 30
// 202220775 박민정
#version 410 core

in vec3 normal; // 전달된 normal을 받기 위해 in 변수를 전역변수로 선언
in vec3 worldPos; //다음 vertex shader로 부터 interpolation된 점들의 3차원 좌표를 얻기 위해 in변수로 vec3 worldPos를 선언
in vec2 texCoord; //2.vertex shader에서 보낸 texCoord를 in으로 받아옴
out vec4 out_Color;

//먼저 몇 가지 uniform 변수를 만듭니다.
uniform vec3 diffColor  = vec3(1.0, 0.5, 0.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos   = vec3(10.0, 20.0, 10.0);
uniform vec3 cameraPos;    // uniform 변수로 float shininess, vec3 cameraPos, vec3 lightPos
uniform float shininess;
uniform sampler2D diffTex; // 1.먼저 texture map을 uniform으로 받아오기 위해 uniform 변수를 선언


void main(void)
{
    //main 함수 속에서, light 방향 (보통 L이라는 이름으로 함)을 계산. lightPos에서 worldPos를 빼고 normalize 하면 됩니다.
    vec3 L = normalize(lightPos - worldPos);
    //길이를 1로 만들기 위해 normalize 하여 다른 변수(보통 N이라고 합니다.)에 저장.
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPos - worldPos); //물체의 한 점(worldPos)에서 카메라 위치 (cameraPos) 쪽으로의 vector를 계산하고 normalize하여 V 방향을 계산
    vec3 R = normalize( 2.0 * dot(N, L) * N - L );//V 방향이 N을 법선 벡터로 가지는 면에 반사된 방향(reflection 방향, R)을 계산
    
    //색상을 저장하기 위한 변수 C를 만들고 diffuse reflection을 계산하여 초기화
    // 3.diffuse reflection을 계산하는 과정에서 color 자리에 texture에서 얻은 값을 사용
    vec3 C = (texture(diffTex, texCoord).rgb) * max( dot(N, L), 0.0 ) * lightColor;

    // specular reflection을 계산하고 그 결과를 C에 더합니다.
    vec3 specular = lightColor * pow( max(dot(R, V), 0.0), shininess );
    C+=specular;
    
    // 27번 과제에서 적용한 gamma transform을 적용한 후 1을 붙여 out_Color로 보냅니다.
    vec3 gamma = pow(C, vec3(1.0/2.2));
    out_Color = vec4(gamma, 1.0);
}
