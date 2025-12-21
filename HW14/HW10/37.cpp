// 컴퓨터 그래픽스 과제 38 
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
#include "triMesh.hpp" // triMesh.hpp 파일을 include
#include <glm/gtc/type_ptr.hpp> //vlaue_ptr 사용
using namespace glm;

GLuint programID = 0;
GLuint shadowProgram = 0;
GLuint textureID; //전역 변수로 GLuint textureID를 선언
GLuint bumpTexID = 0; // bump map texture용 전역 변수 선언

GLuint shadowTex = 0; // 전역변수로 shadowTex, shadowDepth, shadowFBO를 GLuint로 선언합
GLuint shadowDepth = 0;
GLuint shadowFBO = 0;

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

TriMesh model;      // 전역변수로 TriMesh model;을 선언
TriMesh floorModel; //다음 바닥을 만들기 위해 전역변수로 TriMesh floorModel; 을 선언

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
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "BAKMINJEONG-202220775", NULL, NULL);
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
        "/Users/emfpdlzj/Desktop/Computer-Graphics/HW14/HW10/shader.vs",
        "/Users/emfpdlzj/Desktop/Computer-Graphics/HW14/HW10/shader.fs"
    );
    shadowProgram = loadShaders(
        "/Users/emfpdlzj/Desktop/Computer-Graphics/HW14/HW10/shadow.vs",
        "/Users/emfpdlzj/Desktop/Computer-Graphics/HW14/HW10/shadow.fs"
    );

    loadJ3A("/Users/emfpdlzj/Desktop/Computer-Graphics/HW14/HW10/dwarf.j3a");
    //J3A를 읽어 온 후에, 얻은 정보로 간단히 buffer를 만들고 그릴수 있음
    model.create(nVertices[0], vertices[0], normals[0], texCoords[0], nTriangles[0], triangles[0]);

    // 초기화 과정에서 바닥은 큰 사각형이므로 다음 코드를 이용하여 만듬
    floorModel.create(
        { {-100,-1,-100}, {100,-1,-100}, {-100,-1,100}, {100,-1,100} },
        { {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0} },
        { {0,0}, {1,0}, {0,1}, {1,1} },
        { {0,1,2}, {1,2,3} }
    );

    //loadJ3A를 한 직후 다음 stb image를 이용하여 diffuse texture를 읽어 옴
    std::string basePath = "/Users/emfpdlzj/Desktop/Computer-Graphics/HW14/HW10/";

    //기존에 texture를 읽어오던 부분을 수정
    textureID = loadTexture(basePath + diffuseMap[0]);
    // bump mapping을 위해 bump map texture를 읽고 준비
    bumpTexID = loadTexture(basePath + bumpMap[0]);

    // 여 texture를 사용하는 framebuffer object를 만듬
    glGenTextures(1, &shadowTex);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 1024, 1024, 0, GL_RGB,
                 GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &shadowDepth);
    glBindTexture(GL_TEXTURE_2D, shadowDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 1024, 1024, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);// framebuffer object를 사용
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         shadowTex, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                         shadowDepth, 0);
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("FBO Error\n");
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

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
    printf("fov(after)=%.6f\n", fov); //디버깅
}
void render(GLFWwindow* window){
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glViewport(0, 0, 1024, 1024);
    glClearColor(0.2f, 0.7f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //35번 과제 shadowViewMat, shadowProjMat 만드는 과정을 진행
    mat4 shadowProjMat = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 10.0f, 50.0f);
    mat4 shadowViewMat = lookAt(lightPos, vec3(0.0f, 0.0f, 0.0f),
                                vec3(0.0f, 1.0f, 0.0f));

    glUseProgram(shadowProgram);
    mat4 shadowMVP = shadowProjMat * shadowViewMat;
    GLint locShadowMVP = glGetUniformLocation(shadowProgram, "shadowMVP");
    glUniformMatrix4fv(locShadowMVP, 1, GL_FALSE, value_ptr(shadowMVP));

    model.draw();
    floorModel.draw();

    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.2f, 0.7f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);

    //shadowBiasMVP 계산/전달
    mat4 biasMatrix = translate(vec3(0.5f, 0.5f, 0.5f)) * scale(vec3(0.5f, 0.5f, 0.5f));
    mat4 shadowBiasMVP = biasMatrix * shadowMVP;
    GLint locShadowBiasMVP = glGetUniformLocation(programID, "shadowBiasMVP");
    glUniformMatrix4fv(locShadowBiasMVP, 1, GL_FALSE, value_ptr(shadowBiasMVP));

    //Rendering 과정에서 glDrawElements 전에 이 texture를 slot 0번에 bind
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLint locTex = glGetUniformLocation(programID, "diffTex");
    glUniform1i(locTex, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bumpTexID);
    GLint locBumpTex = glGetUniformLocation(programID, "bumpTex");
    glUniform1i(locBumpTex, 1);

    // shadowTex를 slot2에 bind
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    GLint locShadowTex = glGetUniformLocation(programID, "shadowTex");
    glUniform1i(locShadowTex, 2);
    
    // orbit camera: 카메라 위치 계산
    vec4 cam = vec4(0.0f, 0.0f, camDist, 1.0f);
    mat4 rotY = rotate(mat4(1.0f), theta, vec3(0,1,0));
    mat4 rotX = rotate(mat4(1.0f), phi,   vec3(1,0,0));
    vec3 cameraPos = vec3(rotX * rotY * cam);

    viewMat = lookAt(cameraPos, vec3(0,0,0), vec3(0,1,0));

    float aspect = (height>0) ? (width/(float)height) : 1.0f;
    projMat = perspective(fov, aspect, 0.01f, 100.0f);
    
    // projMat/viewMat을 shader uniform으로 전달해야 zoom이 화면에 반영
    GLint locView = glGetUniformLocation(programID, "viewMat");
    GLint locProj = glGetUniformLocation(programID, "projMat");
    glUniformMatrix4fv(locView, 1, GL_FALSE, value_ptr(viewMat));
    glUniformMatrix4fv(locProj, 1, GL_FALSE, value_ptr(projMat));
    
    // shadowProjMat, shadowViewMat은 위에서 이미 계산됨 (light 기준)
    GLint locShadowProjMat = glGetUniformLocation(programID, "shadowProjMat");
    GLint locShadowViewMat = glGetUniformLocation(programID, "shadowViewMat");
    glUniformMatrix4fv(locShadowProjMat, 1, GL_FALSE, value_ptr(shadowProjMat));
    glUniformMatrix4fv(locShadowViewMat, 1, GL_FALSE, value_ptr(shadowViewMat));

    GLint locLightPos   = glGetUniformLocation(programID, "lightPos");
    GLint locLightColor = glGetUniformLocation(programID, "lightColor");
    glUniform3fv( locLightPos,   1, &lightPos[0] );
    glUniform3fv( locLightColor, 1, &lightColor[0] );

    GLint locShine = glGetUniformLocation(programID, "shininess");
    glUniform1f( locShine, shininess[0] );

    GLint locCamPos = glGetUniformLocation(programID, "cameraPos");
    glUniform3fv( locCamPos, 1, &cameraPos[0] );

    GLint locDiffTexEnabled = glGetUniformLocation(programID, "diffTexEnabled");
    GLint locBumpTexEnabled = glGetUniformLocation(programID, "bumpTexEnabled");
    GLint locColor = glGetUniformLocation(programID, "color");

    glUniform1i(locDiffTexEnabled, 1);
    glUniform1i(locBumpTexEnabled, 1);
    glUniform3fv(locColor, 1, &diffuseColor[0].x);
    model.draw();

    vec3 floorColor = vec3(0.85f, 0.85f, 0.85f);
    glUniform1i(locDiffTexEnabled, 0);
    glUniform1i(locBumpTexEnabled, 0);
    glUniform3fv(locColor, 1, &floorColor[0]);
    floorModel.draw();

    glfwSwapBuffers(window);
}
