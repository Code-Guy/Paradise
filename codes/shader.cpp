#include "shader.h"
#include "global.h"

#include <stdio.h>
#include <string.h>

using namespace std;

Shader::Shader()
{
	m_shaderProg = 0;
}

Shader::~Shader()
{
	for (ShaderObjList::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
	{
		glDeleteShader(*it);
	}

	if (m_shaderProg != 0)
	{
		glDeleteProgram(m_shaderProg);
		m_shaderProg = 0;
	}
}

bool Shader::Init()
{
	m_shaderProg = glCreateProgram();

	if (m_shaderProg == 0) {
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}
	
	return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType, const char* pFilename)
{
	string s;

	if (!ReadFile(pFilename, s)) {
		return false;
	}

	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		return false;
	}

	// Save the shader object - will be deleted in the destructor
	m_shaderObjList.push_back(ShaderObj);

	const GLchar* p[1];
	p[0] = s.c_str();
	GLint Lengths[1] = { (GLint)s.size() };

	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling '%s': '%s'\n", pFilename, InfoLog);
		return false;
	}

	glAttachShader(m_shaderProg, ShaderObj);
	return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize(bool isValidate)
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glLinkProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		return false;
	}
	if (isValidate)
	{
		Validate();
	}
	// Delete the intermediate shader objects that have been added to the program
	for (ShaderObjList::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++) {
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	return GLCheckError();
}

bool Shader::Validate()
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		return false;
	}
	return true;
}

void Shader::Enable()
{
	glUseProgram(m_shaderProg);
}


GLint Shader::GetUniformLocation(const char* pUniformName)
{
	GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

	if (Location == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
}

GLint Shader::GetProgramParam(GLint param)
{
	GLint ret;
	glGetProgramiv(m_shaderProg, param, &ret);
	return ret;
}