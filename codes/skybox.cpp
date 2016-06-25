#include "skybox.h"
#include "global.h"
#include "asset.h"

using namespace std;

Skybox::Skybox()
{
	this->camera = Asset::GetInstance()->GetCamera();

	skyboxShader = NULL;
	cubemapTexture = NULL;
	mesh = NULL;
}

Skybox::~Skybox()
{
	SAFE_DELETE(skyboxShader);
	SAFE_DELETE(cubemapTexture);
	SAFE_DELETE(mesh);
}

bool Skybox::Init(const string& Directory, std::string suffix)
{
	skyboxShader = new SkyboxShader();

	if (!skyboxShader->Init()) {
		printf("Error initializing the skybox technique\n");
		return false;
	}

	skyboxShader->Enable();
	skyboxShader->SetTextureUnit(0);

	cubemapTexture = new CubemapTexture(Directory, suffix);

	if (!cubemapTexture->Load()) {
		return false;
	}

	mesh = Asset::GetInstance()->GetMesh("Resources/model/sphere.obj");

	return true;
}

void Skybox::Render()
{
	skyboxShader->Enable();

	GLint oldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	GLint oldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	glm::mat4 MVP = camera->VP * glm::translate(camera->GetPosition()) * glm::scale(100.0f, 100.0f, 100.0f);
	glm::mat4 LightMVP = glm::mat4(1.0f);
	glm::mat4 M = glm::mat4(1.0f);

	cubemapTexture->Bind(GL_TEXTURE0);
	mesh->Render(skyboxShader, 1, &MVP, &LightMVP, &M);  

	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}
