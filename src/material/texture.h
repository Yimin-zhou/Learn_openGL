#pragma once

#include <filesystem>

class Texture 
{
public:
	Texture(const std::filesystem::path& path);
	~Texture();

	void bind(uint32_t slot = 0) const;
	void unbind();

	void setActive();

	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }

private:
	uint32_t m_TextureID;
	std::filesystem::path m_FilePath;

	int m_Width;
	int m_Height;
	int m_BPP;

	unsigned char* m_LocalBuffer;
};