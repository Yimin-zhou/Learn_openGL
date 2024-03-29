#include "shadow.h"

Shadow::Shadow(uint32_t width, uint32_t height) : m_shadowMap(0), m_depthMapFBO(0), m_width(width), m_height(height)
{
	createShadowMap();
}

void Shadow::renderShadowMap(std::vector<Model>& models, std::shared_ptr<Shader> shader, const glm::vec3& lightDir)
{
	glViewport(0, 0, m_width, m_height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	shader->bind();

	// only for directional light
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
	glm::mat4 lightView = glm::lookAt(glm::normalize(lightDir) * 5.0f,
		glm::vec3(0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	m_lightSpaceMatrix = lightProjection * lightView;

	shader->setUniform("projectionViewMatrix", m_lightSpaceMatrix);

	for (auto& model : models)
	{
		for (const auto& mesh : model.getMeshes())
		{
			// draw mesh
			glBindVertexArray(mesh.getVao());
			for (const auto& entry : mesh.getEntries())
			{
				shader->setUniform("modelMatrix", model.getModelMatrix());
				// draw submesh
				glDrawElementsBaseVertex(GL_TRIANGLES,
					entry.numIndices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(uint32_t) * entry.baseIndex),
					entry.baseVertex);
			}
			glBindVertexArray(0);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::createShadowMap()
{ 
	glGenFramebuffers(1, &m_depthMapFBO);
	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
