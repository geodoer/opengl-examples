#pragma once

#define ASSERT(x) if (!(x)) __debugbreak()
    //__debugbreak()是VS MSVC调试器的中断语句

/* 反斜杠后面不能有空格 */
#define GLCall(x) do { \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
 } while (0)

inline void GLClearError() {
    /* 循环获取错误(即清除) */
    while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] "
            << "(" << error << "): " //错误码
            << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
/* 如何使用“错误码”
 * 1. 在glew.h中，搜索1280的十六进制数（即0x0500），定位到错误名GL_INVALID_ENUM
        #define GL_INVALID_ENUM 0x500
 * 2. 然后在docs.gl/gl4/glGetError文档中找到这个错误，即可知道对应的错误详情
 */