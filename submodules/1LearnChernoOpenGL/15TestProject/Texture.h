#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	
	unsigned char* m_LocalBuffer;	//内存当中存储纹理
	int m_Width, m_Height;
	int m_BPP;	//bits per-pixel, 每个像素的比特大小
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};