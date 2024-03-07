#include "GL/glew.h"
#include "stb_image.h"

#include <iostream>

#include "texture.h"

Texture::Texture(const std::filesystem::path& path)
	: m_filePath(path), m_textureID(0), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
{
	loadTextureFromFile(path.string().c_str());
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_textureID);
}

void Texture::loadTextureFromFile(const char* path)
{
	stbi_set_flip_vertically_on_load(0);
	m_localBuffer = stbi_load(path, &m_width, &m_height, &m_BPP, 4);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// Mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (m_localBuffer)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(m_localBuffer);
	}
	else
	{
		std::cout << "Failed to load texture: " << path << std::endl;
	}

}

void Texture::bind(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

