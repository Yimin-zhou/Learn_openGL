#pragma once

#include <filesystem>

class Texture 
{
public:
	Texture(const std::filesystem::path& path);
	~Texture();

	void bind(uint32_t slot = 0) const;
	void unbind();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

private:
	uint32_t m_textureID;
	std::filesystem::path m_filePath;

	int m_width;
	int m_height;
	int m_BPP;

	unsigned char* m_localBuffer;
};