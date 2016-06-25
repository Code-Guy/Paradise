#include "handblock.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>
#include "asset.h"

HandBlock::HandBlock()
{
	this->camera = Asset::GetInstance()->GetCamera();

	angle = 0;
	animateType = No;
	offset = BEGIN_OFFSET;

	//ÉèÖÃµÆ¹âshader
	simpleLightShader = new SimpleLightShader;
	if (!simpleLightShader->Init())
	{
		printf("Error initializing the simple lighting shader\n");
		return;
	}
	simpleLightShader->Enable();
	simpleLightShader->SetColorTextureUnit(0);

	dl.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	dl.AmbientIntensity = 0.8f;
	dl.DiffuseIntensity = 0.6f;
	dl.Direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	simpleLightShader->SetDirectionalLight(dl);

	axis = Asset::GetInstance()->GetMesh("Resources/model/axis.obj", false);
}

HandBlock::~HandBlock()
{
	delete simpleLightShader;
}

void HandBlock::Logic(float deltaTime)
{
	float animationSpeed = ANIMATION_SPEED * deltaTime / STD_DELTA_TIME;
	glm::vec3 step = (END_OFFSET - BEGIN_OFFSET) / (1.0f * (END_ROTATION - BEGIN_ROTATION) / animationSpeed);

	if (animateType == DropOut)
	{
		offset += step;
		angle += animationSpeed;
		if (angle >= END_ROTATION)
		{
			animateType = DropIn;
		}
	}
	else if (animateType == DropIn)
	{
		offset -= step;
		angle -= animationSpeed;
		if (angle <= 0)
		{
			animateType = No;
			angle = 0;
			offset = BEGIN_OFFSET;
		}
	}

	M = glm::rotate(-angle, glm::vec3(1.0f, 0.0f, 0.0f));
	WVP = camera->P * camera->CalcOriginCameraSpaceV(offset) * M;
	LightWVP = glm::mat4(1.0f);
}

void HandBlock::Render()
{
	simpleLightShader->Enable();
	simpleLightShader->SetEyeWorldPos(camera->GetPosition());

	glClear(GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mesh->Render(simpleLightShader, 1, &WVP, &LightWVP, &M, texture);
	glDisable(GL_BLEND);

	glm::mat4 axisM = glm::mat4(1.0f);
	glm::mat4 axisWVP = camera->O * camera->CalcCameraSpaceV(glm::vec3(-0.9f, 0.85f, -5.0f)) * axisM;
	glm::mat4 axisLightWVP = glm::mat4(1.0f);

	axis->Render(simpleLightShader, 1, &axisWVP, &axisLightWVP, &axisM);
}

void HandBlock::MousePressEvent(QMouseEvent *evt)
{
	Hit();
}

void HandBlock::SetMesh(Mesh *mesh)
{
	this->mesh = mesh;
}

void HandBlock::SetTexture(Texture2D *texture)
{
	this->texture = texture;
}

Texture2D *HandBlock::GetTexture()
{
	return texture;
}

Mesh *HandBlock::GetMesh()
{
	return mesh;
}

void HandBlock::Hit()
{
	animateType = DropOut;
}