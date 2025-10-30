    #version 410 core
    // 1. vertex shader에서 uniform 변수 선언
    layout(location=0) in vec3 in_Position;
    layout(location=1) in vec2 in_TexCoord;   // 4. location 1, vec2형식 선언

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
    // 3. modelMat도 단위행렬로 초기화(mat4(1))
    uniform mat4 modelMat = mat4(1);

    out vec2 texCoord; // 5. texCoord 라는 output 변수를 선언

    void main(){
        // 6. n_TexCoord를 texCoord에 대입하여 fragment shader로 보내지도록 합니다.
        texCoord = in_TexCoord;
        
        // 7. homogeneous coordinate로 바꾸고 modelMat으로 world 좌표 계산
        vec4 worldPos = modelMat * vec4(in_Position, 1.0);
        
        // 8. viewMat, projMat 순서로 곱해 최종 클립좌표 계산
        gl_Position = projMat * viewMat * worldPos;
        
    }
