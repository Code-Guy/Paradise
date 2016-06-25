#include "skyboxshader.h"
#include "global.h"
#include <glm/gtc/type_ptr.hpp>

SkyboxShader::SkyboxShader()
{

}

bool SkyboxShader::Init()
{
	if (!Shader::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Resources/shader/Skybox.vs")) {
		return false;
	}
	if (!AddShader(GL_FRAGMENT_SHADER, "Resources/shader/Skybox.fs")) {
		return false;
	}
	if (!Finalize()) {
		return false;
	}

	m_textureLocation = GetUniformLocation("gCubemapTexture");
	if (m_textureLocation == INVALID_UNIFORM_LOCATION) {
			return false;
	}

	return true;
}

void SkyboxShader::SetTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_textureLocation, textureUnit);
}