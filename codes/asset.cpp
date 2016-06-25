#include "asset.h"
#include <QFileInfo>

using namespace std;

Asset *Asset::instance = NULL;

Asset::Asset()
{
	//初始化相机
	camera = new Camera(glm::vec3(0, 5, 10), glm::vec3(0.01, 0, 0), glm::vec3(0, 1, 0));
	textures.clear();
	meshes.clear();
}

Asset::~Asset()
{
	for (map< string, Texture2D * >::iterator ti = textures.begin(); ti != textures.end(); )
	{
		delete ti->second;
		ti = textures.erase(ti);
	}

	for (map< string, Mesh * >::iterator mi = meshes.begin(); mi != meshes.end(); )
	{
		delete mi->second;
		mi = meshes.erase(mi);
	}

	delete camera;
}

Texture2D *Asset::GetTexture(std::string texFileName, GLuint minFilter, GLuint magFilter)
{
	if (textures.find(texFileName) == textures.end())
	{
		textures[texFileName] = new Texture2D(texFileName, minFilter, magFilter);
	}
	return textures[texFileName];
}

Mesh *Asset::GetMesh(std::string meshFileName, bool normalize)
{
	if (meshes.find(meshFileName) == meshes.end())
	{
		meshes[meshFileName] = new Mesh(meshFileName, normalize);
	}
	return meshes[meshFileName];
}

Camera *Asset::GetCamera()
{
	return camera;
}

string Asset::GetTexturePath(const Texture2D *texture)
{
	for (map< string, Texture2D * >::const_iterator ti = textures.begin(); ti != textures.end(); ti++)
	{
		if (ti->second == texture)
		{
			return ti->first;
		}
	}
	return "";
}

string Asset::GetMeshPath(const Mesh *mesh)
{
	for (map< string, Mesh * >::const_iterator mi = meshes.begin(); mi != meshes.end(); mi++)
	{
		if (mi->second == mesh)
		{
			return mi->first;
		}
	}
	return "";
}

QString Asset::GetTextureName(const Texture2D *texture)
{
	string texturePath = GetTexturePath(texture);
	return QFileInfo(QString::fromStdString(texturePath)).baseName();
}

QString Asset::GetMeshName(const Mesh *mesh)
{
	string meshPath = GetMeshPath(mesh);
	return QFileInfo(QString::fromStdString(meshPath)).baseName();
}

Asset *Asset::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Asset();
	}
	return instance;
}