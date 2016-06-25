#ifndef _SIMPLELIGHTSHADER_H
#define _SIMPLELIGHTSHADER_H

#include "shader.h"
#include "light.h"

class SimpleLightShader : public Shader {
public:
	SimpleLightShader();

	static const unsigned int MAX_POINT_LIGHTS = 2;
	static const unsigned int MAX_SPOT_LIGHTS = 2;

	virtual bool Init();

	void SetColorTextureUnit(unsigned int TextureUnit);
	void SetDirectionalLight(const DirectionalLight& Light);
	void SetPointLights(unsigned int NumLights, const PointLight* pLights);
	void SetSpotLights(unsigned int NumLights, const SpotLight* pLights);
	void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
	void SetMatSpecularIntensity(float Intensity);
	void SetMatSpecularPower(float Power);
	void SetMaterialColor(glm::vec4 ambientColor, glm::vec4 diffuseColor, glm::vec4 specularColor);
	void SetUseDiffuseTexture(bool useDiffuseTexture);

private:
	GLuint m_colorTextureLocation;
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_numPointLightsLocation;
	GLuint m_numSpotLightsLocation;
	GLuint m_useDiffuseTextureLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	} m_dirLightLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightsLocation[MAX_POINT_LIGHTS];

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		GLuint Direction;
		GLuint Cutoff;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_spotLightsLocation[MAX_SPOT_LIGHTS];

	struct 
	{
		GLuint ambientColor;
		GLuint diffuseColor;
		GLuint specularColor;
	} m_materialColorLocation;
};

#endif //_SIMPLELIGHTSHADER_H