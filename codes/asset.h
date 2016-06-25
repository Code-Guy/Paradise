#ifndef _ASSET_H
#define _ASSET_H

#include "texture2d.h"
#include "mesh.h"
#include "camera.h"

#include <map>

class Asset
{
public:
	Asset();
	~Asset();

	static Asset *GetInstance();

	Texture2D *GetTexture(std::string texFileName, GLuint minFilter, GLuint magFilter);
	Mesh *GetMesh(std::string meshFileName, bool normalize = true);
	Camera *GetCamera();

	std::string GetTexturePath(const Texture2D *texture);
	std::string GetMeshPath(const Mesh *mesh);

	QString GetTextureName(const Texture2D *texture);
	QString GetMeshName(const Mesh *mesh);

private:
	std::map< std::string, Texture2D * > textures;
	std::map< std::string, Mesh * > meshes;
	Camera *camera;

	static Asset *instance;
};

#endif //_ASSET_H