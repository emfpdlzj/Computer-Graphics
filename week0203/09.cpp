// 컴퓨터 그래픽스 과제 9
// 평행육면체(parallelepiped)의 부피를 계산. 부피는 (a x b) · c
// 입력 값에서 a과 b가 바뀌면 부피가 음수가 나올 수 있음. 출력은 절대값 (abs)를 사용하여 항상 양수가 나오도록 처리
#include <iostream>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

int main(){
    vec3 a,b,c;
    cin>>a.x>>a.y>>a.z>>b.x>>b.y>>b.z>>c.x>>c.y>>c.z;
    const double eps = 1e-6f;   // 오차 보정 용 값
    
    float res = glm::abs(dot(cross(a, b), c)); //(a x b) · c
    if (res < eps) res = 0.0f;  // 거의 0이면 0으로 보정
    
    cout << res << endl;
    return 0;
}

