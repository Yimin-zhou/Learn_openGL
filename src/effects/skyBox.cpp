#include "skyBox.h"

#include <GL/glew.h>
#include <iostream>
#include <stb_image.h>


SkyBox::SkyBox() 
	: m_cubemapTextureID(0), m_irradianceMapID(0), m_prefilterMapID(0), m_brdfLUTTexture(0), m_vao(0), m_vbo(0)
{
}

void SkyBox::initData(const std::vector<std::filesystem::path>& faces)
{
	loadCubemap(faces);
	createIrradianceMap();
	createPrefilterMap();
	createBRDFLUTTexture();
	// data by learnopengl
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

void SkyBox::draw(glm::mat4 view, glm::mat4 projection, std::shared_ptr<Shader> shader)
{
	glDepthFunc(GL_LEQUAL);

	shader->bind();
	shader->setUniform("view", glm::mat4(glm::mat3(view)));
	shader->setUniform("projection", projection);

	glBindVertexArray(m_vao);
	glBindTextureUnit(0, m_cubemapTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS); 
}

void SkyBox::loadCubemap(const std::vector<std::filesystem::path>& faces)
{
	glGenTextures(1, &m_cubemapTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureID);

	int width, height, nrChannels;
	for (uint32_t i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].string().c_str(), &width, &height, &nrChannels, 3);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i].string() << std::endl;
			stbi_image_free(data);
		}
	}

	// create mipmaps for the cubemap
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyBox::createIrradianceMap()
{
	glGenTextures(1, &m_irradianceMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMapID);

	int width = 64;
	int height = 64;
	for (GLuint i = 0; i < 6; ++i) 
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SkyBox::createPrefilterMap()
{
	glGenTextures(1, &m_prefilterMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMapID);

	int width = 128;
	int height = 128;

	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void SkyBox::createBRDFLUTTexture()
{
	glGenTextures(1, &m_brdfLUTTexture);
	glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);

	int width = 512;
	int height = 512;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RG, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void SkyBox::convolveCubemap(std::shared_ptr<Shader> computeShader)
{
	computeShader->bind();

	// bind the cubemap
	glBindTextureUnit(0, m_cubemapTextureID);
	glBindImageTexture(1, m_irradianceMapID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

	// dispatch compute shader
	glDispatchCompute(64 / 16, 64 / 16, 6);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	computeShader->unbind();
}

void SkyBox::prefilterCubemap(std::shared_ptr<Shader> computeShader)
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	uint32_t mipLevels = 5;
	uint32_t maxResolution = 2048;
	for (uint32_t mip = 0; mip < mipLevels; ++mip) {
		// Start from 2048 and halve the dimensions for each subsequent mip level
		uint32_t width = maxResolution * std::pow(0.5, mip); // 2048, 1024, 512, 256, 128
		uint32_t height = maxResolution * std::pow(0.5, mip);

		computeShader->bind();
		computeShader->setUniform("roughness", (float)mip / (mipLevels - 1.0f));

		glBindTextureUnit(0, m_cubemapTextureID);
		glBindImageTexture(1, m_prefilterMapID, mip, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

		glDispatchCompute((width + 15) / 16, (height + 15) / 16, 6);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		computeShader->unbind();
	}
}

void SkyBox::generateBRDFLUTTexture(std::shared_ptr<Shader> computeShader)
{
	computeShader->bind();

	glBindImageTexture(0, m_brdfLUTTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
	glDispatchCompute(512 / 16, 512 / 16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	computeShader->unbind();
}

