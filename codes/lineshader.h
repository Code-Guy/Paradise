#ifndef _LINESHADER_H
#define _LINESHADER_H

#include "shader.h"
#include <glm/glm.hpp>

class LineShader : public Shader
{
public:
	LineShader();

	virtual bool Init();

	void SetMVP(glm::mat4 MVP);
	void SetLineColor(glm::vec4 lineColor);

private:
	GLuint m_MVPLocation;
	GLuint m_LineColorLocation;
};

#endif //_LINESHADER_H