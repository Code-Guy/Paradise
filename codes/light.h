#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>

struct BaseLight
{
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = glm::vec3(1.0f, 1.0f, 1.0f);
	}
};

struct DirectionalLight : public BaseLight
{        
	glm::vec3 Direction;

	DirectionalLight()
	{
		Direction = glm::vec3(-1.0f, -1.0f, 1.0f);
		AmbientIntensity = 0.5f;
		DiffuseIntensity = 0.8f;
	}
};

struct PointLight : public BaseLight
{
	glm::vec3 Position;
	float Distance;

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight()
	{
		AmbientIntensity = 0.1f;
		DiffuseIntensity = 0.3f;
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
		Distance = 10.0f;
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
};

struct SpotLight : public PointLight
{
	glm::vec3 Direction;
	float Cutoff;

	SpotLight()
	{
		AmbientIntensity = 0.1f;
		DiffuseIntensity = 0.3f;
		Direction = glm::vec3(0.0f, -1.0f, 0.0f);
		Cutoff = 60.0f;
	}
};

#endif //_LIGHT_H