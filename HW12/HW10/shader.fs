// 컴퓨터 그래픽스 과제 33
// 202220775 박민정
#version 410 core

in vec3 normal; // 전달된 normal을 받기 위해 in 변수를 전역변수로 선언
in vec3 worldPos; //다음 vertex shader로 부터 interpolation된 점들의 3차원 좌표를 얻기 위해 in변수로 vec3 worldPos를 선언
in vec2 texCoord; //vertex shader에서 보낸 texCoord를 in으로 받아옴
out vec4 out_Color;

//먼저 몇 가지 uniform 변수를 만듭니다.
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos   = vec3(10.0, 20.0, 10.0);
uniform vec3 cameraPos;    // uniform 변수로 float shininess, vec3 cameraPos, vec3 lightPos
uniform float shininess;
uniform sampler2D diffTex; // 먼저 texture map을 uniform으로 받아오기 위해 uniform 변수를 선언
uniform sampler2D bumpTex; // bumpmap 을 sampler2D bumpTex로 받아옵니다.

uniform int diffTexEnabled = 1; // H. fragment shader에 정수로 된 uniform 변수 diffTexEnabled 와 bumpTexEnabled
uniform int bumpTexEnabled = 1;
uniform vec3 color = vec3(1.0, 0.5, 0.0); // I. 물체 색상을 지정하는 uniform 변수

const float TEX_DELTA = 0.0001; //TEX_DELTA는 const float TEX_DELTA=0.0001로 앞에 선언 합니다.

// 11주차 강의자료 77쪽을 참고하여 getTBN 함수를 main보다 앞에 정의합니다.
mat3 getTBN(vec3 N){ // in 변수 및 uniform 변수 이름이 다르니 주의
    vec3 Q1 = dFdx(worldPos);
    vec3 Q2 = dFdy(worldPos);
    vec2 st1 = dFdx(texCoord);
    vec2 st2 = dFdy(texCoord);
    float D = st1.s * st2.t - st2.s * st1.t;
    return mat3(
        normalize((Q1 * st2.t - Q2 * st1.t) * D),
        normalize((-Q1 * st2.s + Q2 * st1.s) * D),
        N
    );
}


void main(void)
{
    //main 함수 속에서, light 방향 (보통 L이라는 이름으로 함)을 계산. lightPos에서 worldPos를 빼고 normalize 하면 됩니다.
    vec3 L = normalize(lightPos - worldPos);
    //길이를 1로 만들기 위해 normalize 하여 다른 변수(보통 N이라고 합니다.)에 저장.
    vec3 N = normalize(normal);

    //    J. TBN을 계산하는 부분부터 bump mapping이 적용된 N을 계산하는 부분까지를 bumpTexEnabled가 0이 아닌 경우에만 수행되도록
    if( bumpTexEnabled != 0 ){ // bumpTexEnable로 bump mapping 여부를 조정
        mat3 TBN = getTBN(N);
        float Bu = texture(bumpTex, texCoord + vec2(TEX_DELTA, 0.0)).r
                 - texture(bumpTex, texCoord - vec2(TEX_DELTA, 0.0)).r;
        float Bv = texture(bumpTex, texCoord + vec2(0.0, TEX_DELTA)).r
                 - texture(bumpTex, texCoord - vec2(0.0, TEX_DELTA)).r;
        vec3 bumpVec = vec3(-Bu * 15.0, -Bv * 15.0, 1.0);
        N = normalize(TBN * bumpVec);
    }

    vec3 V = normalize(cameraPos - worldPos); //물체의 한 점(worldPos)에서 카메라 위치 (cameraPos) 쪽으로의 vector를 계산하고 normalize하여 V 방향을 계산
    vec3 R = normalize( 2.0 * dot(N, L) * N - L );//V 방향이 N을 법선 벡터로 가지는 면에 반사된 방향(reflection 방향, R)을 계산

    //  diffuse color를 저장하기 위한 변수를 만들고, 물체 색상이 저장된 uniform 변수로 초기화
    vec3 baseColor = color;

    // L. diffTexEnabled가 0이 아니면 이 지역변수 값을 texture에서 얻어온 값으로 바꿈
    if( diffTexEnabled != 0 ){
        baseColor = texture(diffTex, texCoord).rgb;
    }
    
    //색상을 저장하기 위한 변수 C를 만들고 diffuse reflection을 계산하여 초기화
    //diffuse reflection을 계산하는 과정에서 color 자리에 texture에서 얻은 값을 사용
    //  M. diffuse reflection을 계산하는 과정에 사용되는 color 값을 이 지역변수로 바꿈
    vec3 C = baseColor * max( dot(N, L), 0.0 ) * lightColor;
    
    // specular reflection을 계산하고 그 결과를 C에 더합니다.
    vec3 specular = lightColor * pow( max(dot(R, V), 0.0), shininess );
    C+=specular;
    
    // 27번 과제에서 적용한 gamma transform을 적용한 후 1을 붙여 out_Color로 보냅니다.
    vec3 gamma = pow(C, vec3(1.0/2.2));
    out_Color = vec4(gamma, 1.0);
}
