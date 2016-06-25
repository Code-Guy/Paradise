#ifndef _SKYBOXSHADER_H
#define _SKYBOXSHADER_H

#include "shader.h"
#include <glm/glm.hpp>

class SkyboxShader : public Shader
{
public:
	SkyboxShader();

	virtual bool Init();
	void SetTextureUnit(unsigned int textureUnit);

private:
	GLuint m_textureLocation;
};

#endif //_SKYBOXSHADER_H