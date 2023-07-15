#### 版本二：将着色器代码抽到文件当中
老师习惯把着色器文件存在`res/shaders/`目录下

老师习惯把顶点着色器、片元着色器放在一个文本文件`*.shader*`当中进行管理

- 因为它们一般成对出现，分成两个文件存，很不直观

例如上一个例子

1. 将顶点着色器、片元着色器放在一个文件当中`res/shaders/Basic.shader`

```glsl
#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
}
```

2. C++代码

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while(getline(stream, line))
	{
		if(line.find("#shader") != std::string::npos)
		{
			if(line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if(line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return {
		ss[0].str(),
		ss[1].str()
	};
}

//...

int main()
{
	//...
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	//相对路径，在 项目属性>调试>工作目录，设置
	std::cout << source.VertexSource << std::endl;
	std::cout << source.FragmentSource << std::endl;

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	while(!glfwWindowShouldClose(window)) {
		//...
	}

	//...
	
	glDeleteProgram(shader); //删除着色器程序
	return 0;
}
```
