#pragma once

/*
 *\brief 负责管理OpenGL的索引缓冲区
 */
class IndexBuffer
{
private:
	unsigned int m_RendererID;	//索引缓冲区的ID
	unsigned int m_Count;		//索引个数
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	//绑定
	void Bind() const;
	//解绑
	void Unbind() const;
	//获得索引的个数
	inline unsigned int GetCount() const { return m_Count; }
};