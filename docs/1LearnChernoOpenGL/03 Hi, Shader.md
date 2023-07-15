如果你没有给OpenGL提供自己的着色器，一些显卡驱动实际上会为你提供一个默认的着色器。因此，上面的代码已经能绘制出一个三角形了。

着色器是一个运行在显卡上的程序，它可以在显卡上以一种非常特殊又非常强大的方式运行。

- Shader很容易让人联想到光源，觉得和着色有关，但它只是在显卡上运行的一个程序而已，和实际的图形没有任何关系

我们可以在计算机上以文本或字符串的形式编写着色器的代码，然后通过OpenGL的API把它们发送到显卡上，它能够在显卡上编译、链接和运行。是的，它运行在GPU上，而不像C++程序运行在CPU上。

顶点着色器

- 它会为我们试图渲染的每个顶点调用，主要目的是告诉OpenGL，这个顶点在屏幕空间的什么位置
- 例如当前的例子，我们有3个顶点，因此会为3个顶点分别调用一次顶点着色器

片段着色器

- 为每个需要光栅化的像素运行一次，决定每个像素的颜色
- 例如当前的例子，片段着色器可能会被调用数万次，这取决于此三角形在屏幕上占据了多少空间


#### 版本一：字符串形式的着色器
```cpp
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
	//...
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
	    //...
    }

	glDeleteProgram(shader); //删除着色器程序
	//...
}
```

