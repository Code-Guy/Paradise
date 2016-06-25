#ifndef _SHADOWMAPFBO_H
#define _SHADOWMAPFBO_H

#include <GL/glew.h>

class ShadowMapFBO
{
public:
	ShadowMapFBO();
	~ShadowMapFBO();

	bool Init(unsigned int width, unsigned int height);
	void BindForWriting();
	void BindForReading(GLenum textureUnit);

	void ResizeTexture(int width, int height);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};

#endif //_SHADOWMAPFBO_H