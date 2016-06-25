#include "lightshader.h"
#include "global.h"
#include <glm/gtc/type_ptr.hpp>
#include <string.h>

LightShader::LightShader()
{   
}

bool LightShader::Init()
{
	if (!Shader::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "Resources/shader/ShadowMapLighting.vs")) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "Resources/shader/ShadowMapLighting.fs")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_colorTextureLocation = GetUniformLocation("gColorMap");
	m_shadowMapLocation = GetUniformLocation("gShadowMap");
	m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
	m_numPointLightsLocation = GetUniformLocation("gNumPointLights");
	m_numSpotLightsLocation = GetUniformLocation("gNumSpotLights");
	m_shadowMapSizeLocation = GetUniformLocation("gMapSize");

	if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
		m_colorTextureLocation == INVALID_UNIFORM_LOCATION ||
		m_shadowMapLocation == INVALID_UNIFORM_LOCATION ||
		m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
		m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
		m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
		m_numPointLightsLocation == INVALID_UNIFORM_LOCATION ||
		m_numSpotLightsLocation == INVALID_UNIFORM_LOCATION ||
		m_shadowMapSizeLocation == INVALID_UNIFORM_LOCATION) {
			return false;
	}

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation) ; i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		m_pointLightsLocation[i].Color = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
		m_pointLightsLocation[i].Position = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Distance", i);
		m_pointLightsLocation[i].Distance = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Distance == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
				return false;
		}
	}

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation) ; i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
		m_spotLightsLocation[i].Color = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
		m_spotLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
		m_spotLightsLocation[i].Position = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Distance", i);
		m_spotLightsLocation[i].Distance = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
		m_spotLightsLocation[i].Direction = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
		m_spotLightsLocation[i].Cutoff = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
		m_spotLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
		m_spotLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
		m_spotLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
		m_spotLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		if (m_spotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Distance == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
				return false;
		}
	}

	return true;
}

void LightShader::SetColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorTextureLocation, TextureUnit);
}

void LightShader::SetShadowMapTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_shadowMapLocation, TextureUnit);
}

void LightShader::SetDirectionalLight(const DirectionalLight& Light)
{
	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	glm::vec3 Direction = Light.Direction;
	Direction = glm::normalize(Direction);
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void LightShader::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void LightShader::SetMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void LightShader::SetMatSpecularPower(float Power)
{
	glUniform1f(m_matSpecularPowerLocation, Power);
}

void LightShader::SetPointLights(const std::vector<PointLight> &pls)
{
	glUniform1i(m_numPointLightsLocation, pls.size());

	for (unsigned int i = 0 ; i < pls.size() ; i++) {
		glUniform3f(m_pointLightsLocation[i].Color, pls[i].Color.x, pls[i].Color.y, pls[i].Color.z);
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pls[i].AmbientIntensity);
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pls[i].DiffuseIntensity);
		glUniform3f(m_pointLightsLocation[i].Position, pls[i].Position.x, pls[i].Position.y, pls[i].Position.z);
		glUniform1f(m_pointLightsLocation[i].Distance, pls[i].Distance);
		glUniform1f(m_pointLightsLocation[i].Atten.Constant, pls[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear, pls[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp, pls[i].Attenuation.Exp);
	}
}

void LightShader::SetSpotLights(const std::vector<SpotLight> &sls)
{
	glUniform1i(m_numSpotLightsLocation, sls.size());

	for (unsigned int i = 0 ; i < sls.size() ; i++) {
		glUniform3f(m_spotLightsLocation[i].Color, sls[i].Color.x, sls[i].Color.y, sls[i].Color.z);
		glUniform1f(m_spotLightsLocation[i].AmbientIntensity, sls[i].AmbientIntensity);
		glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, sls[i].DiffuseIntensity);
		glUniform3f(m_spotLightsLocation[i].Position,  sls[i].Position.x, sls[i].Position.y, sls[i].Position.z);
		glUniform1f(m_spotLightsLocation[i].Distance,  sls[i].Distance);
		glm::vec3 Direction = sls[i].Direction;
		Direction = glm::normalize(Direction);
		glUniform3f(m_spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
		glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(TO_RADIAN(sls[i].Cutoff)));
		glUniform1f(m_spotLightsLocation[i].Atten.Constant, sls[i].Attenuation.Constant);
		glUniform1f(m_spotLightsLocation[i].Atten.Linear,   sls[i].Attenuation.Linear);
		glUniform1f(m_spotLightsLocation[i].Atten.Exp,      sls[i].Attenuation.Exp);
	}
}

void LightShader::SetShadowMapSize(float Width, float Height)
{
	glUniform2f(m_shadowMapSizeLocation, Width, Height);
}