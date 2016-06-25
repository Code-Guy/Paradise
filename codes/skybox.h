#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "camera.h"
#include "mesh.h"
#include "cubemaptexture.h"
#include "skyboxshader.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	bool Init(const std::string& Directory, std::string suffix);
	void Render();

private:
	SkyboxShader *skyboxShader;
	const Camera *camera;
	CubemapTexture *cubemapTexture;
	Mesh *mesh;
};

#endif //_SKYBOX_H