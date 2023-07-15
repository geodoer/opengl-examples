#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Status: Using GL " << glGetString(GL_VERSION) << std::endl;

    //��������
    float positions[6] = 
    {
        //ֻ�������꣬��ά��
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);               //��GPU�д���һ��buffer������ô�buffer��id
    glBindBuffer(GL_ARRAY_BUFFER, buffer);  //ʹ�������������OpenGL��״̬����һ��ֻ�ܰ�һ��GL_ARRAY_BUFFER��
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //�����ݿ�����GPU���У�������λ�ü��ǰ󶨵�GL_ARRAY_BUFFER

    //���ö����������飬����0����λ��
    glEnableVertexAttribArray(0);
    //����GPU����ǰ���������ڴ沼�֣���positions���ڴ沼��
    //��ǰ��positions��ֻ��λ������
    glVertexAttribPointer(0,//ָ����һ���������ԵĲ���
        2,                  //����λ���Ƕ�ά�ģ�������������size=2����
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),  //һ������Ĵ�С
        0                   //positions��ֻ������һ���������ԣ���˾Ͳ���Ҫλ����
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) 
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //������offset=0�ĵط���ʼ
        //����3������
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}