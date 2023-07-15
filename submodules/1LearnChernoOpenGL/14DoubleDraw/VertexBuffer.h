#pragma once

/*
 *\brief 顶点缓冲区
 *\desc VertexBufferLayout只是描述了顶点的规格，并没有存储顶点属性的数据
 *		而顶点缓冲区正是存储顶点数据的地方
 */
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	/*
	 *\brief 在GPU上创建一个顶点缓冲区，并拷贝数据到GPU
	 *\param	data	顶点数据
	 *\param	size	顶点数据的大小	
	 */
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};