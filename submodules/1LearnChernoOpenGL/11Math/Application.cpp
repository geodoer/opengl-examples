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
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
            //顶点位置xy, UV坐标xy
            -0.5f, -0.5f, 0.0f, 0.0f,   // 0
             0.5f, -0.5f, 1.0f, 0.0f,   // 1
             0.5f,  0.5f, 1.0f, 1.0f,   // 2
            -0.5f,  0.5f, 0.0f, 1.0f    // 3
        };
        //索引数据
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        //# 定义OpenGL如何混合alpha像素
        GLCall(glEnable(GL_BLEND)); //启用混合
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  //RGBA = SrcRgba * GL_SRC_ALPHA + DestRgba * GL_ONE_MINUS_SRC_ALPHA

        //# 顶点数组
        VertexArray va;
        //顶点缓冲区
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        //顶点缓冲区的布局
        VertexBufferLayout layout;
        layout.Push<float>(2);  //顶点位置，类型为vec2f
        layout.Push<float>(2);  //UV坐标，类型为vec2f
        //为顶点数组添加顶点缓冲区，并描述其布局
        va.AddBuffer(vb, layout);

        //# 索引缓冲区
        IndexBuffer ib(indices, 6);

        //正交矩阵（把所有坐标映射到2D平面上，离的远的物体并不会变小）
        glm::mat4 proj = glm::ortho(
            -2.0f, //左边
            2.0f,  //右边
            -1.5f, //底部
            1.5f,   //顶部
            -1.0f,  //远
            1.0f    //近
        );
        //注：遵循4x3的纵横比（将这些数字乘以2，就得到了4x3）
        //顶部到底部有3个单位距离；从左到右4个单位距离

        //# 着色器
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", proj); //传入着色器

        //# 纹理
        Texture texture("res/textures/ChernoLogo.png");
        texture.Bind(0);                        //绑定第0个纹理槽
        shader.SetUniform1i("u_Texture", 0);    //传值给着色器，将使用纹理槽的序号给u_Texture变量

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