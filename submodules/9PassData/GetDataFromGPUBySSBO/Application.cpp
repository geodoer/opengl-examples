/*
 * @Author: geodoer
 * @Time: 2023/7/16
 * @Brief: 通过SSBO，将GPU中的数据拷贝出来，到CPU中使用。本实例以gl_FragCoord为例，将每个像素的gl_FragCoord拷贝出来
 * @Note: 本文借助ChatGPT，不能保证内容的准确性
 * @Desc:
   SSBO, Shader Storage Buffer Object
    简介
        1. 当需要在着色器之间进行数据共享或在着色器内部进行持久化数据存储时, SSBO就能派上用场
        2. SSBO是一种在OpenGL中用于着色器之间共享数据的高级缓冲区类型
        3. OpenGL 4.3及以上版本
        4. 与其他OpenGL缓冲区类型一样，使用SSBO需要小心处理并确保正确的同步和数据管理，以避免潜在的竞争条件和数据冲突
    特点
        1. 灵活的数据共享： SSBO允许在不同的着色器中共享数据。这意味着您可以将数据从一个着色器写入到SSBO中，然后从另一个着色器中读取，实现数据共享和传递
        2. 持久化存储： SSBO中的数据在每次绘制过程中保持不变，即使在多个绘制调用之间也是如此。这使得SSBO成为存储持久化数据的理想选择，比如粒子系统的状态、计算结果等
        3. 高效的原子操作： SSBO支持原子操作，允许多个着色器同时对同一数据进行读写，而无需担心竞争条件
        4. 自定义数据布局： 您可以通过layout限定符在着色器中指定SSBO的数据布局，以满足特定的内存需求
        5. 多线程计算： SSBO常用于OpenGL的计算着色器（Compute Shader）中，用于进行大规模的数据计算和处理\
   
   gl_FragCoord 片元着色器内置变量
        1. 坐标系：窗口左下角为原点，向右+X，向上+Y
        2. 单位：像素。若窗口大小为800*600(Width*Height)，则x∈[0.5, 799.5], y∈[0.5, 599.5]
        3. 类型：vec4, 4个float
            xy是像素中心点坐标，即0.5,1.5,2.5,...
            z片元深度值
            w透视除法后的透视修正系数
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <random>

#include "glerror.h"

const int WIDTH = 6;
const int HEIGHT = 4;
//若修改了WIDTH、HEIGHT，要注意片元着色器中也要适配

const char* vertexShaderSource = R"(
#version 430 core

layout (location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 430 core

// 声明SSBO
layout(std430, binding = 0) buffer TextureBuffer {
    vec4 data[];
};

void main()
{
    ivec2 coord = ivec2(gl_FragCoord.xy);

    int index = coord.x + coord.y * 6; //WIDTH=6，因此y*6
    data[index] = gl_FragCoord;
}
)";

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 在创建窗口之前，设置无边框
    /*为什么要设置无边框？
     若WIDTH=8（很小的情况），窗口右上角的“最小化”、“关闭”按钮会将窗口撑大，导致WIDTH>8
     */
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // 创建窗口并设置OpenGL上下文
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Texture Demo", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 初始化GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 创建顶点着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLCall(glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr));
    GLCall(glCompileShader(vertexShader));

    // 创建片元着色器
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLCall(glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr));
    GLCall(glCompileShader(fragmentShader));

    // 创建着色器程序并链接着色器
    GLuint shaderProgram = glCreateProgram();
    GLCall(glAttachShader(shaderProgram, fragmentShader));
    GLCall(glAttachShader(shaderProgram, vertexShader));
    GLCall(glLinkProgram(shaderProgram));
    GLCall(glUseProgram(shaderProgram));

    // 设置顶点数据
    float vertices[] = {
        -1.0, -1.0,
        1.0, -1.0,
        -1.0, 1.0,
        1.0, 1.0
    };

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 创建一个用于存储纹理数据的Shader Storage Buffer Object（SSBO）
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * WIDTH * HEIGHT, nullptr, GL_STATIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); // 将SSBO绑定到绑定点0

    bool printOnce = false;

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        if (!printOnce) {
            // 将SSBO中的数据拷贝回到CPU端
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

            std::vector<glm::vec4> cpuData;
            cpuData.resize(WIDTH * HEIGHT);
            glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * WIDTH * HEIGHT, cpuData.data());

            //print gl_FragCoord坐标系 左下角为原点，向右+X，向上+Y
            {
                for (int y = HEIGHT-1; y >=0; --y) {
                    std::cout << y;

                    for (int x = 0; x < WIDTH; ++x) {
                        int index = x + y * WIDTH;
                        const auto& v = cpuData[index];
                        std::cout << "\t"
                            << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
                    }

                    std::cout << std::endl;
                }

                std::cout << "y/x";

                for (int x = 0; x < WIDTH; ++x) {
                    std::cout << "\t" << x;
                }
                std::cout << std::endl;
            }

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

            printOnce = true;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理资源
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}