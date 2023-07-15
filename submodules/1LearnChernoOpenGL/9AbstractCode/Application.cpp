#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    
    unsigned char* glVersion;
    GLCall(glVersion = (unsigned char*)glGetString(GL_VERSION));
    std::cout << "Status: Using GL " << glVersion << std::endl;

    {
        //顶点数据
        float positions[] = {
            -0.5f, -0.5f, // 0
            0.5f, -0.5f,  // 1
            0.5f, 0.5f,   // 2
            -0.5f, 0.5f,  // 3
        };
        //索引数据
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        //# 顶点数组
        VertexArray va;
        //顶点缓冲区
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        //顶点缓冲区的布局
        VertexBufferLayout layout;
        layout.Push<float>(2); //此顶点缓冲区只有一个属性，即顶点位置，类型为vec2f
        //为顶点数组添加顶点缓冲区，并描述其布局
        va.AddBuffer(vb, layout);

        //# 索引缓冲区
        IndexBuffer ib(indices, 6);

        //# 着色器
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        //# 解除绑定
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        //# 渲染器
        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        while (!glfwWindowShouldClose(window)) 
        {
            renderer.Clear();

            //# 绑定着色器
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            //# 绑定顶点数组、索引缓冲区
            va.Bind();
            ib.Bind();

            //# 绘制一帧
            renderer.Draw(va, ib, shader);

            if (r > 1.0f) {
                increment = -0.05f;
            }
            else if (r < 0.0f) {
                increment = 0.05f;
            }
            r += increment;

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}