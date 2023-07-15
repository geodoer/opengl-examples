#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

//������ɫ��
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); //����ָ�����͵���ɫ��
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);   //ָ��id��ɫ����Դ��
        //����nullptr����ʾ��src����'\0'���������ֻ�봫��source�е�ĳһ�Σ�����ָ������
    glCompileShader(id);                    //������ɫ��

    //��ȡ����״̬
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);     // ��ȡ��־����

        char* msg = (char*)_malloca(length * sizeof(char)); //��ջ�ϴ���
        glGetShaderInfoLog(id, length, &length, msg);       //��ȡ��־��Ϣ

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << msg << std::endl;

        glDeleteShader(id); // ɾ����ɫ��
        return 0;
    }

    return id;
}

//������ɫ��
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); //��������

    //������ɫ��
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //���ӵ�һ��������
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);     //���ӳ���
    glValidateProgram(program); //��֤�Ƿ���Ч

    //��ʱ����ɾ���ˣ���Ϊ�����Ѿ������ӵ�program������
    //��C++�����obj�м��ļ�һ����vs��fs�Ϳ��Ա�ɾ����
    //����ʵΪ�˷�����ԣ��������ɾ������Ϊ���ǵĿ����ǳ�С
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
            //location == glVertexAttribPointer�ĵ�һ������
            //����������У�������ɫ����λ�ô�����vec2�������vec2 position��׼ȷ���������vec2����������Ҫ�ĳ�gl_Position=vec4(position.xy, 0.0, 1.0);
            //���������дvec4��OpenGL���Զ������ǲ��룬����gl_Position�ͷ�����
        "\n"
        "void main()\n"
        "{\n"
        " gl_Position = position;\n"    //gl_Postion��һ��vec4
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0)out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        " color = vec4(1.0, 0.0, 0.0, 1.0);\n"  //ָ��Ϊ��ɫ
        "}\n";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader); /* ʹ����ɫ������ */

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader); // ɾ����ɫ������

    glfwTerminate();
    return 0;
}