/*
 * @Author: geodoer
 * @Time: 2023/7/15
 * @Brief: 通过Texture2D，向GPU传输一个Float数组
 * @Note: 本文借助ChatGPT，不能保证内容的准确性
 * @Desc:
 *  1. 纹理的大小==窗口大小，因此可以在片元着色器中，直接使用gl_FragCoord来取值
 
        gl_FragCoord 是一个内置的变量，用于在片元着色器中获取当前片元的窗口坐标
        它是一个vec4类型的变量，包含了片元相对于窗口左下角的坐标
        若窗口大小为512x512, 那么gl_FragCoord取值范围为(0,0)到(511, 511)，其坐标值是float类型
            gl_FragCoord.x 片元在窗口的水平坐标（x轴）
            gl_FragCoord.y 表示片元在窗口的垂直坐标（y轴）
            gl_FragCoord.z 表示片元的深度值（在使用深度测试时很有用）
            gl_FragCoord.w 表示透视除法后的透视修正系数

 *  2. 如果OpenGL版本大于3.0，建议使用texelFetch，此函数不进行任何过滤或插值，可以按整数索引原封不动的取像素值
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glerror.h"

const int WIDTH = 512;
const int HEIGHT = 512;

const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core

uniform sampler2D textureData;

void main()
{
    //# 一、取像素的两种策略
    // 1. OpenGL<3.0, 使用texture取像素，texture取像素要[0, 1]的UV坐标，因此需要除以窗口大小
    //vec2 texCoord = vec2(gl_FragCoord.xy / vec2(512, 512)); // 根据实际纹理大小调整
    //float value = texture(textureData, texCoord).r;         // 从纹理中采样
    
    // 2. OpenGL>=3.0，使用texelFetch取像素，用整数索引即可，不需要[0, 1]
    ivec2 texCoord = ivec2(gl_FragCoord.xy); 
    float value = texelFetch(textureData, texCoord, 0).r;
        //第三个参数为lod，表示纹理的细节级别，0表示使用基本级别

    //# 二、为验证OpenGL没有将texture2D中的值归一化到[0,1]：
    // 1. 直接将value显示，如果没有归一化，则是全白色
    //gl_FragColor = vec4(value, value, value, 1.0);

    // 2. 将value除以arraySize进行显示，如果没有归一化，是一个渐变
    float arraySize = 512.0 * 512.0;
    value = value / arraySize;
    gl_FragColor = vec4(value, value, value, 1.0);
}
)";

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

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

    // 创建float数组并传输到纹理
    const int arraySize = WIDTH * HEIGHT;
    float* floatArray = new float[arraySize];
    for (int i = 0; i < arraySize; ++i)
    {
        floatArray[i] = static_cast<float>(i);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WIDTH, HEIGHT, 0, GL_RED, GL_FLOAT, floatArray);
    /* OpenGL在传输float数组到纹理时，并不会自动将其归一化到0到1之间。归一化行为取决于您在创建纹理时所设置的参数
        如果您希望将值归一化到0到1之间，可以选择适当的纹理内部格式和像素格式，或在着色器中手动对纹理数据进行归一化操作
     * 在glTexImage2D函数中，通过设置internalformat参数和format参数，您可以控制数据的归一化方式
        1. 如果internalformat和format都设置为GL_R32F，则表示纹理的内部格式和像素数据的格式都是32位浮点数。
            在这种情况下，传输到GPU的float数组的值不会被归一化，而是直接使用原始的浮点值
        2. 如果internalformat设置为GL_RGBA32F，而format设置为GL_RGBA，则表示纹理的内部格式是32位浮点数，但像素数据的格式是RGBA（每个通道8位）
            在这种情况下，OpenGL会将每个浮点值归一化到0到1之间
     */

    // 设置纹理参数
#if 0
    // 1. 用texture取像素值，用这一段
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#else
    // 2. 用texelFetch取像素值，用这一段
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif

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

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理资源
    delete[] floatArray;
    glDeleteTextures(1, &textureID);
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}