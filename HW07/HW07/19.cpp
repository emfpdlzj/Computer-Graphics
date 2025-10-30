/*
// 컴퓨터 그래픽스 과제 19 - 18번 기반
// 202220775 박민정

#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>          // glm/gtx/transform.h을 include
#include "shader.h"
#include "j3a.hpp"
using namespace glm;

GLuint programID = 0, vertexVBOID = 0, vertexArrayID = 0, indexVBOID = 0;
mat4 viewMat, projMat;

// 1. 먼저 전역변수(float)로 camDist, phi, theta, fov 을 선언합니다.
float camDist = 4.0f;     // 원점으로부터 카메라 거리
float phi     = 0.25f;     // x축 회전(상하)
float theta   = -0.5f;     // y축 회전(좌우)
float fov     = 1.04719755f; // 60도 (라디안)

void render(GLFWwindow* window);

int main(void){
    if(!glfwInit()) return 0;

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "박민정-202220775", NULL, NULL);
    if(!window){ glfwTerminate(); return 0; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

#ifdef _MSC_VER
    glewInit();
#endif

    // 셰이더 / 모델 로드
    programID = loadShaders(
        "/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/shader.vs",
        "/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/shader.fs"
    );
    loadJ3A("/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/bunny.j3a");

    // VAO/VBO 세팅
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    glGenBuffers(1, &vertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec3), vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);                         // layout(location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(u32vec3), triangles[0], GL_STATIC_DRAW);

    while(!glfwWindowShouldClose(window)){
        render(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void render(GLFWwindow* window){
    // 화면 사이즈 / 기본 상태
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);

    // orbit camera: 카메라 위치 계산
    vec4 cam = vec4(0.0f, 0.0f, camDist, 1.0f);          // 회전 전 카메라 위치
    mat4 rotY = rotate(mat4(1.0f), theta, vec3(0,1,0));  // y축 회전
    mat4 rotX = rotate(mat4(1.0f), phi,   vec3(1,0,0));  // x축 회전
    vec3 cameraPos = vec3(rotX * rotY * cam);            // 최종 위치

    // Viewing transform은 glm::lookAt 함수를 사용하여 만듬
    viewMat = lookAt(cameraPos, vec3(0,0,0), vec3(0,1,0));
    float aspect = (height>0) ? (width/(float)height) : 1.0f;
    //Projection matrix는 glm::perspective를 이용하여 계산
    projMat = perspective(fov, aspect, 0.01f, 100.0f);

    // 계산한 두 행렬을 각각 uniform 변수인 viewMat과 projMat으로 보냄
    GLint locV = glGetUniformLocation(programID, "viewMat");
    GLint locP = glGetUniformLocation(programID, "projMat");
    glUniformMatrix4fv(locV, 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(locP, 1, GL_FALSE, &projMat[0][0]);

    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}
*/
