#ifndef _SHADER_H
#define _SHADER_H

#include <list>
#include <GL/glew.h>

typedef std::list<GLuint> ShaderObjList;

class Shader
{
public:
	Shader();
	virtual ~Shader();

	virtual bool Init();
	void Enable();

	bool Validate();

protected:
	bool AddShader(GLenum ShaderType, const char* pFilename);
	bool Finalize(bool isValidate = true);

	GLint GetUniformLocation(const char* pUniformName);
	GLint GetProgramParam(GLint param);

	GLuint m_shaderProg;    

private:
	ShaderObjList m_shaderObjList;
};

#endif //_SHADER_H