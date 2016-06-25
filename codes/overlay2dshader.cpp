#include "overlay2dshader.h"
#include "global.h"
#include <glm/gtc/type_ptr.hpp>

Overlay2DShader::Overlay2DShader()
{

}

bool Overlay2DShader::Init()
{
	if (!Shader::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Resources/shader/Overlay2D.vs")) {
		return false;
	}
	if (!AddShader(GL_FRAGMENT_SHADER, "Resources/shader/Overlay2D.fs")) {
		return false;
	}
	if (!Finalize()) {
		return false;
	}

	m_ScreenSizeLocation = GetUniformLocation("ScreenSize");
	if (m_ScreenSizeLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	m_textureLocation = GetUniformLocation("myTextureSampler");
	if (m_textureLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	m_colorLocation = GetUniformLocation("color");
	if (m_colorLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void Overlay2DShader::SetScreenSize(int width, int height)
{
	glUniform2f(m_ScreenSizeLocation, width, height);
}

void Overlay2DShader::SetTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_textureLocation, textureUnit);
}

void Overlay2DShader::SetColor(glm::vec4 color)
{
	glUniform4fv(m_colorLocation, 1, glm::value_ptr(color));
}