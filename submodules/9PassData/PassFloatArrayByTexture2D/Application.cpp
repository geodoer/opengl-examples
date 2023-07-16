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
 *  3. 用SSBO将value拷贝出来，检查数据传输的准确性
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
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

uniform sampler2D textureData;
uniform vec2 windowSize; //窗口大小，(width, height)

// 声明SSBO
layout(std430, binding = 0) buffer TextureBuffer 
{
    float data[];
};

void main()
{
    ivec2 texCoord = ivec2(gl_FragCoord.xy); 

    //# 一、取像素的两种策略
    // 1. OpenGL<3.0, 使用texture取像素，texture取像素要[0, 1]的UV坐标，因此需要除以窗口大小
    float value = texture(textureData, gl_FragCoord.xy / windowSize).r;
    
    // 2. OpenGL>=3.0，使用texelFetch取像素，用整数索引即可，不需要[0, 1]
    //float value = texelFetch(textureData, texCoord, 0).r;
        //第三个参数为lod，表示纹理的细节级别，0表示使用基本级别

    int index = texCoord.x + texCoord.y * int(windowSize.x);
    data[index] = value;

    float arraySize = windowSize.x * windowSize.y;
    value = value / arraySize;
    gl_FragColor = vec4(value, value, value, 1.0);
}
)";

void print(float* data) {
    std::cout << "y/x";
    for (int x = 0; x < WIDTH; ++x) {
        std::cout << "\t\t" << x;
    }
    std::cout << std::endl;

    for (int y = 0; y < HEIGHT; ++y) {
        std::cout << y;

        for (int x = 0; x < WIDTH; ++x) {
            int index = x + y * WIDTH;
            std::cout << "\t\t" << data[index];
        }

        std::cout << std::endl;
    }
}

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
    if (WIDTH < 400) 
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
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
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, &vertexShaderSource);

    // 创建片元着色器
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, &fragmentShaderSource);

    // 创建着色器程序并链接着色器
    GLuint shaderProgram = glCreateProgram();
    GLCall(glAttachShader(shaderProgram, fragmentShader));
    GLCall(glAttachShader(shaderProgram, vertexShader));
    GLCall(glLinkProgram(shaderProgram));
    GLCall(glUseProgram(shaderProgram));
    
    // 为uniform windowSize设置窗口大小
    GLint windowSizeLocation = glGetUniformLocation(shaderProgram, "windowSize");
    glUniform2f(windowSizeLocation, WIDTH, HEIGHT);

    // 创建float数组并传输到纹理
    const int arraySize = WIDTH * HEIGHT;
    std::random_device rd;  // 获取一个真随机数种子
    std::mt19937 gen(rd()); // 使用 Mersenne Twister 算法
    std::uniform_real_distribution<float> dis(0.0f, arraySize);

    float* floatArray = new float[arraySize];
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int idx = x + y * WIDTH;
            floatArray[idx] = dis(gen); //随机数
        }
    }
    print(floatArray);
    std::cout << std::endl;

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

    // 创建一个用于存储纹理数据的Shader Storage Buffer Object（SSBO）
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * arraySize, nullptr, GL_STATIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); // 将SSBO绑定到绑定点0

    bool doOnce = false;

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        if (!doOnce) 
        {
            // 将SSBO中的数据拷贝回到CPU端
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

            float* cpuData = new float[arraySize];
            glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * arraySize, cpuData);
            print(cpuData);
            delete[] cpuData;

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

            doOnce = true;
        }

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