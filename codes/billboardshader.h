#ifndef _BILLBOARDSHADER_H
#define _BILLBOARDSHADER_H

#include "shader.h"
#include <glm/glm.hpp>

class BillboardShader : public Shader
{
public:
	BillboardShader();

	virtual bool Init();
	void SetMV(glm::mat4 MV);
	void SetP(glm::mat4 P);
	void SetTextureUnit(unsigned int textureUnit);
	
private:
	GLuint m_MVLocation;
	GLuint m_PLocation;
	GLuint m_textureLocation;
};

#endif //_BILLBOARDSHADER_H