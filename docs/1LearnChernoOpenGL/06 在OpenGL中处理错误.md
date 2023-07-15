
## glGetError

### 简易使用
```cpp
static void GLClearError()
{
	while(glGetError() != GL_NO_ERROR);
}

static void GLCheckError()
{
	while(GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] ("
				<< error 
				<< ")"
				<< std::endl;
	}
}

//使用，用两个函数夹住OpenGL的函数
GLClearError(); //清除在之前的所有错误
glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
GLCheckError(); //检查上一句话是否报错
```

### 封装成断言的形式
```cpp
#define ASSERT(x) if(!(x)) __debugbreak();
	//__debugbreak()是VS MSVC调试器的中断语句

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while(GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] ("
				<< error 
				<< ")"
				<< ": " << function
				<< " " << file
				<< " " << line
				<< std::endl;
		return false;
	}
	
	return true;
}

//使用OpenGL函数时，用GLCall包装一下就行
GLCall(unsigned int program = glCreateProgram());
GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));
```

### OpenGL错误码
`GLenum error = glGetError()`会返回一个错误码，如何查看它的错误信息呢？

例如，错误码1280

1.在`glew.h`中，搜索1280的十六进制数（即`0x0500`），就能获得一个
```cpp
#define GL_INVALID_ENUM 0x500
```

2.然后在`docs.gl/gl4/glGetError`文档中找到这个错误，即可知道对应的错误详情

## glDebugMessageCallback
> 在OpenGL4.3以后，提供了回调函数让我们获取OpenGL的信息