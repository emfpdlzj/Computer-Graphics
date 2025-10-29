/*
// 컴퓨터 그래픽스 과제 16 - 과제12 개선
// 202220775 박민정
#include <GLFW/glfw3.h>
#include "shader.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "j3a.hpp"   // 3.j3a.hpp파일을 include
// j3a변수 nVertices[0], vertices[0], nTriangles[0], triangles[0]
using namespace std;
using namespace glm;

void render(GLFWwindow* window);

GLuint program = 0;
GLuint vertexArrayID = 0;
GLuint vertexVBOID = 0;
GLuint indexVBOID = 0;

int main(void) {
    if (!glfwInit()) return 0;
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(800, 800, "BAKMINJEONG202220775", NULL, NULL);
    if (!window) { glfwTerminate(); return 0; }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
#ifdef _MSC_VER
    glewInit();
#endif
    // 5.loadJ3A 함수를 사용하여 bunny.j3a 파일을 읽습니다.
    loadJ3A("/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/bunny.j3a");
    
    program = loadShaders( //6. 이때 program을 load하는 부분은 남아있어야 함
                          "/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/shader.vs",
                          "/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/shader.fs"
                          );
    //7.loadJ3A를 통해 전역변수에 읽어 온 데이터를 buffer에 집어넣습니다.
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    glGenBuffers(1, &vertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glGenBuffers(1, &indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);
    
    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void render(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(program);
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    //8.render 과정에서 기존의 glDrawElement 부분을 지우고, 강의 자료 85쪽과 같이 glDrawElement를 호출
    glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);
    
    glfwSwapBuffers(window);
}

*/
