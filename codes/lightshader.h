#ifndef _LIGHTSHADER_H
#define _LIGHTSHADER_H

#include <vector>
#include "shader.h"
#include "light.h"

class LightShader : public Shader {
public:
	LightShader();

	static const unsigned int MAX_POINT_LIGHTS = 10;
	static const unsigned int MAX_SPOT_LIGHTS = 10;

	virtual bool Init();

	void SetColorTextureUnit(unsigned int TextureUnit);
	void SetShadowMapTextureUnit(unsigned int TextureUnit);
	void SetDirectionalLight(const DirectionalLight& Light);
	void SetPointLights(const std::vector<PointLight> &pls);
	void SetSpotLights(const std::vector<SpotLight> &sls);
	void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
	void SetMatSpecularIntensity(float Intensity);
	void SetMatSpecularPower(float Power);
	void SetShadowMapSize(float Width, float Height);

private:
	GLuint m_colorTextureLocation;
	GLuint m_shadowMapLocation;
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_numPointLightsLocation;
	GLuint m_numSpotLightsLocation;
	GLuint m_shadowMapSizeLocation;

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
		GLuint Distance;
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
		GLuint Distance;
		GLuint Direction;
		GLuint Cutoff;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_spotLightsLocation[MAX_SPOT_LIGHTS];
};

#endif //_LIGHTSHADER_H