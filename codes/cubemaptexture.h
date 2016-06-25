#ifndef _CUBEMAPTEXTURE_H
#define  _CUBEMAPTEXTURE_H

#include <string>
#include <GL/glew.h>

class CubemapTexture
{
public:
	CubemapTexture(const std::string& Directory, std::string suffix);

	~CubemapTexture();

	bool Load();
	void Bind(GLenum textureUnit);

private:
	std::string m_fileNames[6];
	GLuint m_textureObj;
};

#endif //_CUBEMAPTEXTURE_H