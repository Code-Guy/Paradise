#ifndef _SHADOWMAPSHADER_H
#define _SHADOWMAPSHADER_H

#include "shader.h"
#include <glm/glm.hpp>

class ShadowMapShader : public Shader
{
public:
	ShadowMapShader();

	virtual bool Init();

	void SetTextureUnit(unsigned int textureUnit);

private:
	GLuint m_textureLocation;
};

#endif //_SHADOWMAPSHADER_H