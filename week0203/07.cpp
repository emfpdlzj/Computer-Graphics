//7주차 과제

#include <iostream> //입출력용
#include <glm/glm.hpp> //glm

using namespace glm;
using namespace std;

int main(void){
    vec3 v1,v2;
    const float eps = 1e-6f; //오차 보정을 위한 값
    float res;
    
    cin>> v1.x >> v1.y >> v1.z >> v2.x >> v2.y >> v2.z; //벡터 값 입력 받음
    if (length(v1) == 0.0f || length(v2) == 0.0f) { //0벡터 입력 경우 처리
            std::cout << 0 << '\n';
            return 0;
        }
    
    float i  = dot(normalize(v1), normalize(v2));    //사잇값 계산
    i=glm::clamp(i,-1.0f, 1.0f);
    
    i = glm::clamp(i, -1.0f, 1.0f); //오차보정
    if (1.0f - i < eps)  i = 1.0f;    // 거의 1이면 1로 조정
    if (i + 1.0f < eps)  i = -1.0f;   //   거의 -1이면 -1로 조정
    
    res = degrees(glm::acos(i)); // degree로 변환. [0,180] 범위
    cout << res; //출력
    return 0;
}

