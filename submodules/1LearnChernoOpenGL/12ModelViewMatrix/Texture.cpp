#include "Texture.h"
#include "vender/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); //垂直翻转纹理 
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
		//想要使用RGBA，则传入4
	
	GLCall(glGenTextures(1, &m_RendererID));			//生成一个纹理
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));	//绑定纹理

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));	//缩小过滤器 线性重采样
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));	//放大过滤器 线性重采样
	/*“嵌入模式”or“环绕模式”
	1. GL_CLAMP_TO_EDGE	嵌入 不希望它扩大区域
	2. 平铺
	 */
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));	//水平环绕(S相当于X) 
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));	//垂直环绕(T相当于Y)
	//注：如果不设置这四个参数，会得到一个黑色的纹理。因为它们没有默认值

	//将数据拷贝到GPU上
	GLCall(glTexImage2D(
		GL_TEXTURE_2D,		//纹理类型
		0,					//level=0，表示它不是一个复合纹理
		GL_RGBA8,			//你希望OpenGL在GPU中如何存储你的纹理 => 按照RGBA8来存储
		m_Width, m_Height,	//宽高
		0,					//像素边框=0
		GL_RGBA,			//传给GPU数据的格式是什么 => RGBA四个通道
		GL_UNSIGNED_BYTE,	//传给GPU数据的类型 => 每个通道是一个unsigned byte
		m_LocalBuffer		//数据（也可以传nullptr，这表示先分配显存，到后面再拷贝）
	)); 
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); //解除绑定

	if (m_LocalBuffer) 
	{
		stbi_image_free(m_LocalBuffer); //删除内存中的数据
		m_LocalBuffer = nullptr;
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer); //删除内存中的数据
		m_LocalBuffer = nullptr;
	}
}

void Texture::Bind(unsigned int slot) const
{
	//激活第slot个纹理槽（这意味着下一个纹理将绑定到第slot个纹理槽上）
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	//将m_RendererID纹理，绑定到激活的槽里（也就是第slot个纹理槽中）
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//注：虽然纹理槽范围是[GL_TEXTURE0, GL_TEXTURE31]，但不一定你的显卡支持，这还是取决于你的显卡
	//	  有32个，是因为32是OpenGL规格允许的最大值
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
