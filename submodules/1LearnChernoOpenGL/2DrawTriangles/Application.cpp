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

    //顶点数组
    float positions[6] = 
    {
        //只包含坐标，二维的
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);               //在GPU中创建一个buffer，并获得此buffer的id
    glBindBuffer(GL_ARRAY_BUFFER, buffer);  //使用这个缓冲区（OpenGL是状态机，一次只能绑定一个GL_ARRAY_BUFFER）
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //将数据拷贝到GPU当中，拷贝的位置即是绑定的GL_ARRAY_BUFFER

    //启用顶点属性数组，索引0，的位置
    glEnableVertexAttribArray(0);
    //告诉GPU，当前缓冲区的内存布局，即positions的内存布局
    //当前，positions中只有位置属性
    glVertexAttribPointer(0,//指定第一个顶点属性的布局
        2,                  //顶点位置是二维的，有两个分量，size=2即可
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),  //一个顶点的大小
        0                   //positions中只包含了一个顶点属性，因此就不需要位移量
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) 
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //缓冲区offset=0的地方开始
        //绘制3个顶点
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}