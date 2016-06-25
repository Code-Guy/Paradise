#include "billboardshader.h"
#include "global.h"
#include <glm/gtc/type_ptr.hpp>

BillboardShader::BillboardShader()
{

}

bool BillboardShader::Init()
{
	if (!Shader::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Resources/shader/Billboard.vs")) {
		return false;
	}
	if (!AddShader(GL_GEOMETRY_SHADER, "Resources/shader/Billboard.gs")) {
		return false;
	}
	if (!AddShader(GL_FRAGMENT_SHADER, "Resources/shader/Billboard.fs")) {
		return false;
	}
	if (!Finalize()) {
		return false;
	}

	m_MVLocation = GetUniformLocation("MV");
	if (m_MVLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}
	m_PLocation = GetUniformLocation("P");
	if (m_PLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}
	m_textureLocation = GetUniformLocation("ColorSampler");
	if (m_textureLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void BillboardShader::SetMV(glm::mat4 MV)
{
	glUniformMatrix4fv(m_MVLocation, 1, false, glm::value_ptr(MV));
}

void BillboardShader::SetP(glm::mat4 P)
{
	glUniformMatrix4fv(m_PLocation, 1, false, glm::value_ptr(P));
}

void BillboardShader::SetTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_textureLocation, textureUnit);
}