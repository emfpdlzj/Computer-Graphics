// 컴퓨터 그래픽스 과제 29
// 202220775 박민정

#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>          // glm/gtx/transform.h을 include
#include "shader.h"
#include "j3a.hpp"
#include "stb_image.h" // stb_image.h를 include 합니다.
using namespace glm;

GLuint programID = 0, vertexVBOID = 0, vertexArrayID = 0, indexVBOID = 0;
// buffer를 한 개 더 생성
GLuint normalVBOID = 0;

GLuint textureID; //전역 변수로 GLuint textureID를 선언
GLuint texVBOID =0; //texture coordinate를 보내기 위한 buffer를 한 개 더 생성

mat4 viewMat, projMat;

// 먼저 전역변수(float)로 camDist, phi, theta, fov 을 선언합니다.
float camDist = 4.0f;          // 원점으로부터 카메라 거리
float phi     = 0.25f;         // x축 회전(상하)
float theta   = -0.5f;         // y축 회전(좌우)
float fov     = 1.04719755f;   // 60도 (라디안)

//  mouse의 움직임을 알기 위해 직전의 pointer 위치를 저장할 전역변수(double) 선언
static double lastX = 0.0, lastY = 0.0;

//전역변수로 lightPos 와 lightColor를 선언하고, 초기 값은 uniform에서의 초기 값과 동일
vec3 lightPos   = vec3(10.0f, 20.0f, 10.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void render(GLFWwindow* window);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);   //  마우스 이동 콜백 함수(정의)
void buttonCallback(GLFWwindow* window, int button, int action, int mods); //  버튼 콜백 함수(정의)

// glfwSetScrollCallback 함수로 scroll wheel에 대한 callback 함수를 등록
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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

    // 콜백 등록
    glfwSetMouseButtonCallback(window, buttonCallback);
    glfwSetCursorPosCallback(window,   cursorCallback);
    glfwSetScrollCallback(window,      scrollCallback);

    // 셰이더 / 모델 로드
    programID = loadShaders(
        "/Users/emfpdlzj/Desktop/Computer-Graphics/HW11/HW10/shader.vs",
        "/Users/emfpdlzj/Desktop/Computer-Graphics/HW11/HW10/shader.fs"
    );
    loadJ3A("/Users/emfpdlzj/Desktop/Computer-Graphics/HW11/HW10/dwarf.j3a");

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    glGenBuffers(1, &vertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec3), vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &normalVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBOID);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec3), normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // texture coordinate buffer 생성
    glGenBuffers(1, &texVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, texVBOID);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(vec2), texCoords[0], GL_STATIC_DRAW);
    // 이 buffer를 vertex attribute 1번으로 보냄
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(u32vec3), triangles[0], GL_STATIC_DRAW);

    //loadJ3A를 한 직후 다음 stb image를 이용하여 diffuse texture를 읽어 옴
    int w = 0, h = 0, n = 0;

    stbi_set_flip_vertically_on_load(true);//stb_image가 그림의 위 아래를 뒤집도록 함.
    void* buf = stbi_load("/Users/emfpdlzj/Desktop/Computer-Graphics/HW11/HW10/dwarfD.jpg", &w, &h, &n, 4);
    printf("width = %d, height =%d \n", w, h); //4번 작업 중 얻어온 w 와 h를 print하여 texture가 잘 읽혔는지 확인

    //  image를 texture로 만든다.
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
    // 마지막에 stbi_image_free를 빠트리지 마십시오.
    stbi_image_free(buf);
    
    glEnable(GL_DEPTH_TEST); // 이 과정을 통해 z-buffer algorithm이 수행되고 뒤의 것을 안 그려 집니다.
    while(!glfwWindowShouldClose(window)){
        render(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//  main 밖에 cursorPosCB 함수 정의
void cursorCallback(GLFWwindow* window, double xpos, double ypos){
    //  왼쪽 버튼이 눌려있는 동안에만 카메라를 움직입니다.
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        //  먼저 window의 크기를 얻어 옵니다 (framebuffer 기준)
        int width = 1, height = 1;
        glfwGetFramebufferSize(window, &width, &height);
        //  theta = -(xpos - lastX)/width * π
        theta += (float)(-(xpos - lastX) / (double)width * 3.1415926535);
        //  phi  = -(ypos - lastY)/height * π
        phi   += (float)(-(ypos - lastY) / (double)height * 3.1415926535);
        lastX = xpos;         //  마지막 좌표 갱신
        lastY = ypos;
    }
}

// mouse button callback: 눌린 순간 pointer 위치를 lastX,lastY에 저장
void buttonCallback(GLFWwindow* window, int button, int action, int mods){
    // if문 속의 button == GLFW_MOUSE_BUTTON_LEFT 이고, action == GLFW_PRESS일 때
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        //  눌린 순간 pointer의 위치를 lastX, lastY에 저장
        glfwGetCursorPos(window, &lastX, &lastY);
    }
}

// zoom 기능은 fov 값을 조절하는 것으로 구현
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    // yoffset 값에 따라 fov 값을 변경
    fov -= (float)(yoffset * 0.005);

    const float EPS     = 0.01f;
    const float FOV_MAX = 3.1415926535f - EPS;
    if(fov > FOV_MAX) fov = FOV_MAX;
    if(fov < EPS)     fov = EPS;
}

void render(GLFWwindow* window){
    // 화면 사이즈 / 기본 상태
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //매번 새로운 z-buffer 상태에서 시작하기 위해
    glUseProgram(programID);
    
    //Rendering 과정에서 glDrawElements 전에 이 texture를 slot 0번에 bind
    glActiveTexture(GL_TEXTURE0);
    //지정한 0번 slot을 shader의 uniform으로 보냄
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLint locTex = glGetUniformLocation(programID, "diffTex");
    glUniform1i(locTex, 0);
    
    // orbit camera: 카메라 위치 계산
    vec4 cam = vec4(0.0f, 0.0f, camDist, 1.0f);          // 회전 전 카메라 위치
    mat4 rotY = rotate(mat4(1.0f), theta, vec3(0,1,0));  // y축 회전
    mat4 rotX = rotate(mat4(1.0f), phi,   vec3(1,0,0));  // x축 회전
    vec3 cameraPos = vec3(rotX * rotY * cam);            // 최종 위치

    viewMat = lookAt(cameraPos, vec3(0,0,0), vec3(0,1,0));

    float aspect = (height>0) ? (width/(float)height) : 1.0f;

    // rendering 부분에서는 이 값을 이용하여 projection matrix를 계산하고, 적용
    projMat = perspective(fov, aspect, 0.01f, 100.0f);
    
    // rendering 과정에서 이 값들을 uniform으로 보냅니다. vec3을 보낼때는 glUniform3fv를 사용하는 것이 일반적
    GLint locLightPos   = glGetUniformLocation(programID, "lightPos"); //추가함
    GLint locLightColor = glGetUniformLocation(programID, "lightColor"); //추가함
    glUniform3fv( locLightPos,   1, &lightPos[0] );                      //추가함
    glUniform3fv( locLightColor, 1, &lightColor[0] );
    
    //그 외에 diffuse color와 shininess도 보내 줌.
    //diffuse color는 diffuseColor[0] 으로 얻어올 수 있고, vec3. shininess 역시 shininess[0]에 들어 있고, type은 float
    GLint locDiffColor = glGetUniformLocation(programID, "diffColor");   //추가함
    glUniform3fv( locDiffColor, 1, &diffuseColor[0].x );                                               //추가함
    GLint locShine = glGetUniformLocation(programID, "shininess");       //추가함
    glUniform1f( locShine, shininess[0] );
    
    //마지막으로 viewing 과정에서 계산한 cameraPos도 shader로 전달
    GLint locCamPos = glGetUniformLocation(programID, "cameraPos");      //추가함
    glUniform3fv( locCamPos, 1, &cameraPos[0] );
    
    
    glBindVertexArray(vertexArrayID);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}
