#ifndef _OVERLAY2DSHADERSHADER_H
#define _OVERLAY2DSHADERSHADER_H

#include "shader.h"
#include <glm/glm.hpp>

class Overlay2DShader : public Shader
{
public:
	Overlay2DShader();

	virtual bool Init();
	void SetScreenSize(int width, int height);
	void SetTextureUnit(unsigned int textureUnit);
	void SetColor(glm::vec4 color);

private:
	GLuint m_ScreenSizeLocation;
	GLuint m_textureLocation;
	GLuint m_colorLocation;
};

#endif //_OVERLAY2DSHADERSHADER_H