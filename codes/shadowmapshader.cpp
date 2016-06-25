#include "shadowmapshader.h"
#include "global.h"
#include <glm/gtc/type_ptr.hpp>

ShadowMapShader::ShadowMapShader()
{

}

bool ShadowMapShader::Init()
{
	if (!Shader::Init())
	{
		return false;
	}
	if (!AddShader(GL_VERTEX_SHADER, "Resources/shader/ShadowMapping.vs")) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "Resources/shader/ShadowMapping.fs")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_textureLocation = GetUniformLocation("gShadowMap");

	if (m_textureLocation == INVALID_UNIFORM_LOCATION) {
			return false;
	}

	return true;
}

void ShadowMapShader::SetTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_textureLocation, textureUnit);
}