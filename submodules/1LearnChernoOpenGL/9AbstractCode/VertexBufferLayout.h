#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

/*
 *\brief ������������
 *\desc	������һ���������ԵĹ��
 *\example
 *	��vector3f => type=float;count=3 �����Դ洢���㡢��������
 *  ��vector2f => type=float,count=2 �����Դ洢UV���꣩
 */
struct VertexBufferElement
{
	unsigned int type;			//�����������ͣ���float uint
	unsigned int count;			//����ά��
	unsigned char normalized;	//�Ƿ��׼��

	//�������ͻ��size
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
 *\brief ���㻺��������
 *\desc ������һ�����㻺���������˼����������ԣ�ÿ���������ԵĹ�����
 */
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;//N����������
	unsigned int m_Stride;						//ÿ������֮��ļ����һ������Ĵ�С��
public:
	VertexBufferLayout(): m_Stride(0) {}

	//Pushһ����������
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false); //��δʵ�ֵ���������
	}

	//Pushһ��float�͵Ķ������ԣ��˶������Ե�ά����count
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;	//һ������Ĵ�С += �����ԵĴ�С
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