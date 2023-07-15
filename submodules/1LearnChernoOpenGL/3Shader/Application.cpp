#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

//编译着色器
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); //创建指定类型的着色器
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);   //指定id着色器的源码
        //传入nullptr，表示以src是以'\0'结束。如果只想传入source中的某一段，可以指定长度
    glCompileShader(id);                    //编译着色器

    //获取编译状态
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);     // 获取日志长度

        char* msg = (char*)_malloca(length * sizeof(char)); //在栈上创建
        glGetShaderInfoLog(id, length, &length, msg);       //获取日志信息

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << msg << std::endl;

        glDeleteShader(id); // 删除着色器
        return 0;
    }

    return id;
}

//创建着色器
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); //创建程序

    //创建着色器
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //附加到一个程序当中
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);     //链接程序
    glValidateProgram(program); //验证是否有效

    //此时可以删除了，因为它们已经被链接到program当中了
    //和C++程序的obj中间文件一样，vs、fs就可以被删除了
    //但其实为了方便调试，这个无需删除，因为它们的开销非常小
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit()) return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Status: Using GL " << glGetString(GL_VERSION) << std::endl;

    float positions[6] =
    {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0)in vec4 position;"
            //location == glVertexAttribPointer的第一个参数
            //在上面代码中，顶点着色器的位置传的是vec2，因此用vec2 position更准确。但如果用vec2，下面则需要改成gl_Position=vec4(position.xy, 0.0, 1.0);
            //但这里可以写vec4，OpenGL会自动帮我们补齐，后面gl_Position就方便了
        "\n"
        "void main()\n"
        "{\n"
        " gl_Position = position;\n"    //gl_Postion是一个vec4
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0)out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        " color = vec4(1.0, 0.0, 0.0, 1.0);\n"  //指定为红色
        "}\n";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader); /* 使用着色器程序 */

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader); // 删除着色器程序

    glfwTerminate();
    return 0;
}