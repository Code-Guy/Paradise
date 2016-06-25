#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H

#include <string>
#include <GL/glew.h>

class Texture2D
{
public:
	Texture2D(const std::string& fileName, GLuint minFilter = GL_LINEAR, GLuint magFilter = GL_LINEAR);
	~Texture2D();

	bool Load(const std::string& fileName, GLuint minFilter, GLuint magFilter);
	void Bind(GLenum textureUnit);
	
	bool IsValid();

	int width;
	int height;

private:
	bool valid;
	GLuint textureObj;
};

#endif //_TEXTURE2D_H