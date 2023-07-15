#pragma once

#include <iostream>
#include "GL/glew.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak()

/* ��б�ܺ��治���пո� */
#define GLCall(x) do { \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
 } while (0)

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

/*
 *\brief ��Ⱦ��
 */
class Renderer
{
public:
    void Clear() const;
    /*
     *\brief ����һ֡
     */
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};