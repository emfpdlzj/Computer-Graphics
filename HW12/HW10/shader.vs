#version 410 core
// vertex shader에서 uniform 변수 선언
layout(location=0) in vec3 in_Position;
layout(location=1) in vec2 in_TexCoord;   // location 1, vec2형식 선언
layout(location=2) in vec3 in_Normal; //vertex attribute로 normal vector를 받기 위해 새로운 in 변수

//행렬 선언, 초기값 지정
uniform mat4 viewMat = mat4(
                            1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,-3,1
                            );
uniform mat4 projMat = mat4(
                            1.299038, 0, 0, 0,
                            0, 1.732051, 0, 0,
                            0, 0, -1.002002, -1.0,
                            0, 0, -0.2002, 0
                            );
//  modelMat도 단위행렬로 초기화(mat4(1))
uniform mat4 modelMat = mat4(1);

out vec2 texCoord; //  texCoord 라는 output 변수를 선언
out vec3 normal; // normal vector를 fragment shader로 보내기 위해 out 변수
out vec3 worldPos; // out 변수로 vec3 worldPos를 선언합

void main(){
    vec4 worldCoord = modelMat * vec4(in_Position, 1.0);  //in_Position을 homogeneous coordinate로 만들고, modelMat을 곱한 결과를 worldCoord
    worldPos = worldCoord.xyz; //worldCoord.xyz를 worldPos에 대입하여 fragment shader로 보냄
            
    normal = normalize( (modelMat * vec4(in_Normal, 0.0)).xyz );
    //in_Normal에 0을 붙여 homogeneous coordinate(vec4)를 만들고
    //이 vec4에 modelMat을 곱하여 world coordinate의 normal vector 방향을 계산
    //xyz 성분만 떼내고 normalize() 함수를 이용해 길이를 1로 만든 후 normal (out변수)에 저장
            
    //texture Coordinate를 fragment shader로 전달
    texCoord = in_TexCoord;
            
    // viewMat, projMat 순서로 곱해 최종 클립좌표 계산
    gl_Position = projMat * viewMat * worldCoord;//1-3. worldCoord.xyz를 worldPos에 대입하여 fragment shader로 보냄
            
}
