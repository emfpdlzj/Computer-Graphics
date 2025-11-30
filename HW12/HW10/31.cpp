// 컴퓨터 그래픽스 과제 33
// 202220775 박민정
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>          // glm/gtx/transform.h을 include
#include "shader.h"
#include "j3a.hpp"
#include "stb_image.h" // stb_image.h를 include 합니다.
#include <string> //str 관련 함수 사용을 위해 include
#include <vector>
#include "triMesh.hpp" // A.triMesh.hpp 파일을 include
using namespace glm;

GLuint programID = 0;

GLuint textureID; //전역 변수로 GLuint textureID를 선언
GLuint bumpTexID = 0; // bump map texture용 전역 변수 선언

mat4 viewMat, projMat;

// 먼저 전역변수(float)로 camDist, phi, theta, fov 을 선언합니다.
float camDist = 4.0f;          // 원점으로부터 카메라 거리
float phi     = 0.25f;         // x축 회전(상하)
float theta   = -0.5f;         // y축 회전(좌우)
float fov     = 1.04719755f;   // 60도 (라디안)

// mouse의 움직임을 알기 위해 직전의 pointer 위치를 저장할 전역변수(double) 선언
static double lastX = 0.0, lastY = 0.0;

//전역변수로 lightPos 와 lightColor를 선언하고, 초기 값은 uniform에서의 초기 값과 동일
vec3 lightPos   = vec3(10.0f, 20.0f, 10.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

TriMesh model;      //A. 전역변수로 TriMesh model;을 선언
TriMesh floorModel; //E. 다음 바닥을 만들기 위해 전역변수로 TriMesh floorModel; 을 선언

void render(GLFWwindow* window);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);   //  마우스 이동 콜백 함수(정의)
void buttonCallback(GLFWwindow* window, int button, int action, int mods); //  버튼 콜백 함수(정의)

// glfwSetScrollCallback 함수로 scroll wheel에 대한 callback 함수를 등록
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

//loadTexture 함수를 선언
GLuint loadTexture(const std::string& filename){ //매개변수는 std::string filename
    int w = 0, h = 0, n = 0;
    //함수 내부 내용은 기존에 texture를 읽어 오는 부분을 거의 그대로 사용
    stbi_set_flip_vertically_on_load(true); //stb_image가 그림의 위 아래를 뒤집도록 함.
    void* buf = stbi_load(filename.c_str(), &w, &h, &n, 4); // 파일 이름 부분은 매개변수로 받은 filename으로부터 c_str을 받아서 사용
    printf("width = %d, height =%d \n", w, h); //4번 작업 중 얻어온 w 와 h를 print하여 texture가 잘 읽혔는지 확인

    GLuint textureID = 0; // textureID를 지역 변수로 선언하여 내부적으로 사용
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//Mipmapping을 적용하기 위해 MIN_FILTER 부분 변경
    //genTexture, texParameteri, glTexImage2D, stbi_image_free 과정은 동일하게 수행
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
    glGenerateMipmap(GL_TEXTURE_2D);//이후 mipmap을 만들기 위해 glTexImage2D 이후에 glGenerateMipmap 함수를 호출

    // 마지막에 stbi_image_free를 빠트리지 마십시오.
    stbi_image_free(buf);

    return textureID; // return 값은 GLuint
}

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
        "/Users/emfpdlzj/Desktop/Computer-Graphics/HW12/HW10/shader.vs",
        "/Users/emfpdlzj/Desktop/Computer-Graphics/HW12/HW10/shader.fs"
    );
    loadJ3A("/Users/emfpdlzj/Desktop/Computer-Graphics/HW12/HW10/dwarf.j3a");
    //C. J3A를 읽어 온 후에, 얻은 정보로 간단히 buffer를 만들고 그릴수 있음
    model.create(nVertices[0], vertices[0], normals[0], texCoords[0], nTriangles[0], triangles[0]);

    //  F. 초기화 과정에서 바닥은 큰 사각형이므로 다음 코드를 이용하여 만듬
    floorModel.create(
        { {-100,-1,-100}, {100,-1,-100}, {-100,-1,100}, {100,-1,100} },
        { {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0} },
        { {0,0}, {1,0}, {0,1}, {1,1} },
        { {0,1,2}, {1,2,3} }
    );

    //loadJ3A를 한 직후 다음 stb image를 이용하여 diffuse texture를 읽어 옴
    std::string basePath = "/Users/emfpdlzj/Desktop/Computer-Graphics/HW12/HW10/";

    //기존에 texture를 읽어오던 부분을 수정
    textureID = loadTexture(basePath + diffuseMap[0]);
    // bump mapping을 위해 bump map texture를 읽고 준비
    bumpTexID = loadTexture(basePath + bumpMap[0]);
    
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
    glClearColor(0.2f, 0.7f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //매번 새로운 z-buffer 상태에서 시작하기 위해
    glUseProgram(programID);
    
    //Rendering 과정에서 glDrawElements 전에 이 texture를 slot 0번에 bind
    glActiveTexture(GL_TEXTURE0);
    //지정한 0번 slot을 shader의 uniform으로 보냄
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLint locTex = glGetUniformLocation(programID, "diffTex");
    glUniform1i(locTex, 0);

    //Rendering 과정에서 역시 이 texture를 shader로 보내야 합니다.
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bumpTexID);
    GLint locBumpTex = glGetUniformLocation(programID, "bumpTex");
    glUniform1i(locBumpTex, 1);

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
    GLint locLightPos   = glGetUniformLocation(programID, "lightPos");
    GLint locLightColor = glGetUniformLocation(programID, "lightColor");
    glUniform3fv( locLightPos,   1, &lightPos[0] );
    glUniform3fv( locLightColor, 1, &lightColor[0] );
    
    //그 외에 diffuse color와 shininess도 보내 줌.
    //diffuse color는 diffuseColor[0] 으로 얻어올 수 있고, vec3. shininess 역시 shininess[0]에 들어 있고, type은 float
    GLint locShine = glGetUniformLocation(programID, "shininess");
    glUniform1f( locShine, shininess[0] );
    
    //마지막으로 viewing 과정에서 계산한 cameraPos도 shader로 전달
    GLint locCamPos = glGetUniformLocation(programID, "cameraPos");
    glUniform3fv( locCamPos, 1, &cameraPos[0] );

    //H. fragment shader에 정수로 된 uniform 변수 diffTexEnabled 와 bumpTexEnabled
    GLint locDiffTexEnabled = glGetUniformLocation(programID, "diffTexEnabled");
    GLint locBumpTexEnabled = glGetUniformLocation(programID, "bumpTexEnabled");
    GLint locColor = glGetUniformLocation(programID, "color");
    
    // N. main 파일의 rendering 부분에 model을 draw하기 직전에 uniform 값인 bumpTexEnabled와 diffTexEnabled를 1로 지정하고 draw
    glUniform1i(locDiffTexEnabled, 1);
    glUniform1i(locBumpTexEnabled, 1);
    glUniform3fv(locColor, 1, &diffuseColor[0].x);
    model.draw(); // D. rendering 과정에서 buffer, vertex array를 bind 하는 부분과 glDrawElements 를 지운 후 다음의 코드로 이 과정을 진행

    // O. 바닥을 그리기 직전에 uniform 값인 bumpTexEnabled와 diffTexEnabled를 0으로 지정
    vec3 floorColor = vec3(0.85f, 0.85f, 0.85f); //O. 이때 uniform인 color 값도 원하는 값(가급적 어둡지 않는 색)으로 지정
    glUniform1i(locDiffTexEnabled, 0);
    glUniform1i(locBumpTexEnabled, 0);
    glUniform3fv(locColor, 1, &floorColor[0]);
    floorModel.draw(); // G. Rendering 과정에서는 역시 floorModel.draw(); 를 통하여 바닥을 그릴 수 있음.

    glfwSwapBuffers(window);
}
