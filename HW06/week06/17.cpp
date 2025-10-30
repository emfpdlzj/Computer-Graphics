/*
// 컴퓨터 그래픽스 과제 18 - 15번기반. 16참고했음.
// 202220775 박민정
#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>  // 3.
#include <glm/gtx/transform.hpp>         // 3.
#include "shader.h"
#include "j3a.hpp"
using namespace glm;

void render(GLFWwindow* window);

GLuint programID = 0, vertexVBOID = 0, vertexArrayID = 0, indexVBOID = 0;
float theta = 0.0f;  // 1.각도를 저장하기 위한 전역변수를 선언하고 0으로 초기화
mat4 viewMat, projMat;

int main(void) {
    if (!glfwInit()) return 0;
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    GLFWwindow* window =
    glfwCreateWindow(800, 600, "박민정-202220775", NULL, NULL);
    if (!window) { glfwTerminate(); return 0; }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
#ifdef _MSC_VER
    glewInit();
#endif
    
    // 셰이더 로드 (17번에서 사용하던 shader.vs / shader.fs)
    programID = loadShaders(
                            "/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/shader.vs",
                            "/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/shader.fs"
                            );
    
    // 16 동일: bunny.j3a 로드
    loadJ3A("/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/bunny.j3a");
    
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    glGenBuffers(1, &vertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec3), vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // layout(location=0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glGenBuffers(1, &indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(u32vec3), triangles[0], GL_STATIC_DRAW);
    
    viewMat = lookAt(vec3(0,0,3), vec3(0,0,0), vec3(0,1,0));
    projMat = perspective(radians(60.0f), 800.0f/600.0f, 0.1f, 10.0f);
    glUseProgram(programID);
    glUniformMatrix4fv(glGetUniformLocation(programID,"viewMat"), 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID,"projMat"), 1, GL_FALSE, &projMat[0][0]);
    
    while(!glfwWindowShouldClose(window)){
        render(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void render(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programID);
    
    theta += 0.01f; //render 함수에서는 각도를 0.01 radian 씩 증가 시킵니다.
    
    //3차원 rotation matrix 생성
    mat4 rotY = rotate(mat4(1.0f), theta, vec3(0.0f, 1.0f, 0.0f));
    
    // 5.vertex shader의 uniform 중 modelMat의 location을 얻습니다.
    GLint loc = glGetUniformLocation(programID, "modelMat");
    //6.이 location에 방금 만든 행렬을 보냅니다.
    glUniformMatrix4fv(loc, 1, GL_FALSE, &rotY[0][0]);
    
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
}
*/
