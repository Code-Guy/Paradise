#include "lineshader.h"
#include "global.h"
#include <glm/gtc/type_ptr.hpp>

LineShader::LineShader()
{

}

bool LineShader::Init()
{
	if (!Shader::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Resources/shader/Line.vs")) {
		return false;
	}
	if (!AddShader(GL_FRAGMENT_SHADER, "Resources/shader/Line.fs")) {
		return false;
	}
	if (!Finalize()) {
		return false;
	}

	m_MVPLocation = GetUniformLocation("MVP");
	if (m_MVPLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	m_LineColorLocation = GetUniformLocation("LineColor");
	if (m_LineColorLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void LineShader::SetMVP(glm::mat4 MVP)
{
	glUniformMatrix4fv(m_MVPLocation, 1, false, glm::value_ptr(MVP));
}

void LineShader::SetLineColor(glm::vec4 lineColor)
{
	glUniform4fv(m_LineColorLocation, 1, glm::value_ptr(lineColor));
}