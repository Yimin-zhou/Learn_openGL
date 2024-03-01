#pragma once

#include <filesystem>

class Texture 
{
public:
	Texture(const std::filesystem::path& path);
	~Texture();

	void loadTextureFromFile(const char* path);

	void bind(uint32_t slot = 0) const;
	void unbind();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

	uint32_t getTextureID() const { return m_textureID; }

private:
	uint32_t m_textureID;
	std::filesystem::path m_filePath;

	int m_width;
	int m_height;
	int m_BPP;

	unsigned char* m_localBuffer;
};