## OpenGL是什么

OpenGL不是一个类库，也不是一个引擎，不是一个框架，非开源
OpenGL只是一个规范，和C++规范差不多

- 它只是提供一些规范。它规定了OpenGL包含了哪些函数，这个函数应该存在，这个函数需要这些参数并且返回这个值
- 它没有提供函数的实现，是由GPU制造商实现。假设你使用的是NVIDIA的显卡，那么OpenGL所定义的函数由英伟达写在NVIDIA的显卡驱动里
- 每个显卡制造商（比如AMD、Intel等）都会有OpenGL的具体实现，而且代码都不同，因此OpenGL不是开源的，GPU制造商不会开源这些代码

## 创建窗口和上下文

在使用OpenGL绘图之前，我们需要一个窗口和OpenGL上下文。我们可以根据Windows API自行创建一个窗口，但这很麻烦，而且写的代码也不能跨平台。市面上有一些第三方库能够帮我们完成这个工作，并且写的代码也能跨平台，如glfw、glut。

本教程采用glfw，下载地址：[An OpenGL library | GLFW](https://www.glfw.org/)

```cpp
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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
```

## 获取OpenGL函数指针

OpenGL只是一个规范，而函数实现则在显卡驱动里，因此我们需要去显卡驱动中取出OpenGL函数的指针。具体而言，在安装完显卡驱动之后，会有一个驱动的动态链接文件，我们需要检索此文件中的所有函数指针，找出自己所需要的。

市面上有很多第三方库能够帮助我们完成这件事情，并且它们也支持跨平台（代码跨平台：一样的代码，拿到不同平台编译也能运行）。如glew、glad等等。

本教程使用glew（The OpenGL Extension Wrangler Library），下载地址：[GLEW: The OpenGL Extension Wrangler Library (sourceforge.net)](https://glew.sourceforge.net/)

使用glew需注意

1. 若使用glew的静态库，需要在引用库之前定义宏`GLEW_STATIC`
2. 必须在创建了窗口、初始化上下文之后（上下文有效），才能调用`glewInit`

```cpp
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

int main(void)
{
	//...
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Status: Using GL " << glGetString(GL_VERSION) << std::endl;

	//...
}
```
