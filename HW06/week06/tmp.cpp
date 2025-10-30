// 컴퓨터 그래픽스 과제 17 - 3차원 토끼(회전 없음, 최소구현)
// 202220775 박민정
#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include "shader.h"
#include "j3a.hpp"   // nVertices[0], vertices[0], nTriangles[0], triangles[0]
#include <iostream>
using namespace std;

void render(GLFWwindow* window);

// OpenGL 리소스 (소문자 카멜케이스)
GLuint program = 0;
GLuint vertexArrayID = 0;
GLuint vertexVBOID   = 0;
GLuint indexVBOID    = 0;

int main() {
    if (!glfwInit()) return 0;

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    GLFWwindow* window =
        glfwCreateWindow(800, 600, "박민정_202220775", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 0; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

#ifdef _MSC_VER
    glewInit();
#endif

    // 0) 셰이더 로드
    program = loadShaders("/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/shader.vs", "/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/shader.fs");
    if (!program) { cerr << "shader load failed\n"; return 0; }

    // 1) bunny 데이터 로드 (경로 맞춰주기)
    if (!loadJ3A("/Users/emfpdlzj/Desktop/25-2CG/6주차/week06/week04-2/bunny.j3a")) { cerr << "bunny.j3a load failed\n"; return 0; }

    // 2) VAO 생성/바인드
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // 3) 위치 VBO
    glGenBuffers(1, &vertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBOID);
    glBufferData(GL_ARRAY_BUFFER,
                 nVertices[0] * sizeof(glm::vec3),
                 vertices[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // layout(location=0)
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0
    );

    // (선택) 텍스처 좌표를 쓰진 않지만 location=1을 안전하게 0으로 고정
    glDisableVertexAttribArray(1);

    // 4) 인덱스(EBO)
    glGenBuffers(1, &indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 nTriangles[0] * sizeof(glm::u32vec3),
                 triangles[0],
                 GL_STATIC_DRAW);

    // 루프
    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }

    // 정리
    glDeleteBuffers(1, &indexVBOID);
    glDeleteBuffers(1, &vertexVBOID);
    glDeleteVertexArrays(1, &vertexArrayID);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void render(GLFWwindow* window) {
    int w, h; glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);

    // 회전 없이, 과제 17 요구대로 model/view/proj는 셰이더 초기값 사용
    glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}
