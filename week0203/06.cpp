//
//  main.cpp
//  project01
//
//  Created by 박민정 on 9/13/25.
//

#include <GLFW/glfw3.h> //프로그램에서 GLFW의 기능을 사용하기 위해 함수들의 기능을 불러옴
void render(GLFWwindow* window); //화면에 필요한 내용을 그리는 함수 Render

static float factor=0;

int main(void){
    if(!glfwInit()) //GLFW라이브러리의 다양한 변수를 초기화함
        return 0;
#ifdef __APPLE__ //컴파일 해야하는 플랫폼 코드. MacOSss
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); //4.1버전 사용 명시
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    //Forward Compatibility를 지정, Core Profile 사용시 표시
#endif
    GLFWwindow* window = glfwCreateWindow(800,600,"BAKMINJEONG202220775", NULL,NULL); //윈도우를 생성, 윈도우 크기 지정, 윈도우 타이틀 제목
    glfwMakeContextCurrent(window); //그림 그릴때 사용할 Context 지정
    glfwSwapInterval(1);
#ifdef _MSC_VER //MS Window에서 꼭 필요한 과정
    glewInit();
#endif
    while(!glfwWindowShouldClose(window)){ //윈도우가 열려있는 동안 반복
        render(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate(); //GLFW 환경을 정리하고 종료
}

void render(GLFWwindow* window){
    factor+=0.01;
    if(factor>1) factor=0;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0,0,width,height); //화면이 지워질 때 사용할 색상 지정
    //0,0부터 w,h까지 그림을 그릴 예정임을 나타냄.
    glClearColor(0.22,0.07, factor,0);//RGBA값. Red, Green, Blue, Alpha값을 지정함.
    glClear(GL_COLOR_BUFFER_BIT); //화면을 지우는 함수. 현재 프레임 버퍼의 값을 앞에 지정한 색상으로 채움.
    glfwSwapBuffers(window); //buffer swap. 지금까지 그리는 과정은 back buffer대상임.
}
