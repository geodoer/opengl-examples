#pragma once

/*
 *\brief ���㻺����
 *\desc VertexBufferLayoutֻ�������˶���Ĺ�񣬲�û�д洢�������Ե�����
 *		�����㻺�������Ǵ洢�������ݵĵط�
 */
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	/*
	 *\brief ��GPU�ϴ���һ�����㻺���������������ݵ�GPU
	 *\param	data	��������
	 *\param	size	�������ݵĴ�С	
	 */
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};