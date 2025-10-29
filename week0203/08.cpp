// 컴퓨터 그래픽스 과제 8
// 평면과 직선의 교점
// 직선과 점이 만나지 않는 경우, 또는 한 점 이상에서 만나는 경우-> 법선 벡터와 직선이 수직이 되는 경우 처리
//  Created by 박민정 on 9/28/25.
#include <iostream> //입출력용
#include <glm/glm.hpp> //glm

using namespace glm;
using namespace std;

int main(void){
    vec3 n, p0, p1, p2;
    cin>> p0.x >> p0.y >> p0.z >> n.x >> n.y >> n.z;
    cin >> p1.x >> p1.y >> p1.z >> p2.x >> p2.y >>  p2.z;
    
    const float eps = 1e-7f;    // 0.00001 과 같은 미세 오차 보정 값

    if (length(n) == 0.0f){    // 법선이 0인 경우 처리
        cout << "Cannot solve";
        return 0;
    }
    //𝛼 = (p0-p1)·n / (l·n) 계산하기
    vec3 l = p2 - p1;   // 직선 방향벡터 l
    float a = dot(l, n);    // (l·n) 계산
    float b = dot(p0 - p1, n);  // (p0-p1)·n 계산
    
    if (glm::abs(a) < eps){ // (l·n)가 0이면 평면과 직선이 평행.
        cout << "Cannot solve";
        return 0;
    }
    
    // t = ((p0 - p1) · n) / ((p2 - p1) · n)
    float t = b/a; //parameter t
    vec3 p = p1 + t * l;    // 교점 p 계산
    
    cout << p.x << ' ' << p.y << ' ' << p.z << endl;
    return 0;
}


