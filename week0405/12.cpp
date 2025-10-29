/*
// 컴퓨터 그래픽스 과제 12
#include <GLFW/glfw3.h>
#include "shader.h"
#include <vector>
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>         // 콘솔 입출력

using namespace std;
using namespace glm;
void render(GLFWwindow* window);

GLuint program = 0, vertexBuffer = 0, vertexArray = 0,  indeices =0; //indices 선언
int n = 0;

int main(void) {
    cin >> n;

    if (!glfwInit()) return 0;
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // 4.1 Core
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    GLFWwindow* window =
    glfwCreateWindow(800, 800, "BAKMINJEONG202220775", NULL, NULL);
    if (!window) { glfwTerminate(); return 0; }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
#ifdef _MSC_VER
    glewInit();
#endif
    // 초기화 코드
    program = loadShaders("/Users/emfpdlzj/Desktop/25-2CG/4주차/week04-2/week04-2/shader.vs", "/Users/emfpdlzj/Desktop/25-2CG/4주차/week04-2/week04-2/shader.fs"); //프로그램을 읽는다.
    
    const float vertices[9] = { //삼각형의 세 점의 좌표를 저장하는 배열을 만듬
        -0.7f, -0.7f, 0.0f,
        0.7f, -0.7f, 0.0f,
        0.0f,  0.7f, 0.0f
    };

    vector<vec3> points;
    for (int i = 0; i < n; ++i) {
        float theta = (2.0f*3.1415926f*i)/n;
        points.push_back(vec3(cos(theta), sin(theta), 0.0f));
    }
    
    // vertexBuffer에 Array buffer를 만들고 만든 배열의 데이터를 넣는다.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // points 데이터
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3), points.data(), GL_STATIC_DRAW);

    // vertexArray에 vertex array를 만든다.
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    // vertexArray가 바인드 되어 있는 상태에서 vertexBuffer를 Vertex attribute 0번에 연결
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                          0,                // location=0
                          3,                // xyz
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),// stride
                          0          // offset
                          );

    unsigned int indices[3];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    //정N각형을 삼각형들로 쪼개는 인덱스(uvec3)
    vector<uvec3> triangles;
    for (int i = 0; i < n - 2; ++i) {
        triangles.push_back(uvec3(0, i + 1, i + 2)); // 0,1,2 / 0,2,3 / ...
    }

    // element array buffer 만들고 배열정보 넘겨저장
    glGenBuffers(1, &indeices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeices);
    // triangles업로드
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(uvec3), triangles.data(), GL_STATIC_DRAW);
    
    while(!glfwWindowShouldClose(window)){ //윈도우가 열려있는 동안 반복
        render(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate(); //GLFW 환경을 정리하고 종료
}

void render(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);//화면이 지워질 때 사용할 색상 지정
    //0,0부터 w,h까지 그림을 그릴 예정임을 나타냄.
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//RGBA값. Red, Green, Blue, Alpha값을 지정함.
    glClear(GL_COLOR_BUFFER_BIT); //화면을 지우는 함수. 현재 프레임 버퍼의 값을 앞에 지정한 색상으로 채움.
    
    // 삼각형그리기
    glUseProgram(program);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeices);
    //전체 삼각형 수 * 3 만큼 그리기
    glDrawElements(GL_TRIANGLES, (GLsizei)((n - 2) * 3), GL_UNSIGNED_INT, 0);
    
    glfwSwapBuffers(window); //buffer swap. 지금까지 그리는 과정은 back buffer대상임.
}
*/
