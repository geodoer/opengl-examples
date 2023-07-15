#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

/*
 *\brief 顶点属性描述
 *\desc	描述了一个顶点属性的规格
 *\example
 *	如vector3f => type=float;count=3 （可以存储顶点、法向量）
 *  如vector2f => type=float,count=2 （可以存储UV坐标）
 */
struct VertexBufferElement
{
	unsigned int type;			//它的数据类型，如float uint
	unsigned int count;			//它的维度
	unsigned char normalized;	//是否标准化

	//根据类型获得size
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

/*
 *\brief 顶点缓冲区布局
 *\desc 描述了一个顶点缓冲区包含了几个顶点属性，每个顶点属性的规格如何
 */
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;//N个顶点属性
	unsigned int m_Stride;						//每个顶点之间的间隔（一个顶点的大小）
public:
	VertexBufferLayout(): m_Stride(0) {}

	//Push一个顶点属性
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false); //还未实现的数据类型
	}

	//Push一个float型的顶点属性，此顶点属性的维度是count
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;	//一个顶点的大小 += 此属性的大小
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};