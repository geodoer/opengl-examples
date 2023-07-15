#pragma once

/*
 *\brief �������OpenGL������������
 */
class IndexBuffer
{
private:
	unsigned int m_RendererID;	//������������ID
	unsigned int m_Count;		//��������
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	//��
	void Bind() const;
	//���
	void Unbind() const;
	//��������ĸ���
	inline unsigned int GetCount() const { return m_Count; }
};