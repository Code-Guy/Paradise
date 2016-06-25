#ifndef _MESH_H
#define	_MESH_H

#include <map>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <glm/glm.hpp>

#include "texture2d.h"
#include "global.h"
#include "shader.h"

#define INDEX_BUFFER 0    
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3    
#define WVP_MAT_VB   4
#define LIGHTWVP_MAT_VB 5
#define WORLD_MAT_VB 6

struct Vertex
{
	glm::vec3 m_pos;
	glm::vec2 m_tex;
	glm::vec3 m_normal;

	Vertex() {}
	Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
	{
		m_pos    = pos;
		m_tex    = tex;
		m_normal = normal;
	}
};

struct MaterialColor
{
	MaterialColor(aiColor4D ambientColor = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f),
		aiColor4D diffuseColor = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f),
		aiColor4D specularColor = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f))
		:ambientColor(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a), 
		diffuseColor(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a), 
		specularColor(specularColor.r, specularColor.g, specularColor.b, specularColor.a)
	{

	}
	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
};

class Mesh
{
public:
	Mesh(const std::string& Filename, bool normalize);
	~Mesh();

	bool Load(const std::string& Filename);
	void Render(Shader *shader, unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* LightWVPMats, const glm::mat4* WorldMats, Texture2D *texture = NULL);

	static glm::mat4 CalcM(glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 scalar = glm::vec3(1.0f, 1.0f, 1.0f));//¼ÆËãÄ£ÐÍ¾ØÕó

private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(const aiMesh* paiMesh,
		std::vector<glm::vec3>& Positions,
		std::vector<glm::vec3>& Normals,
		std::vector<glm::vec2>& TexCoords,
		std::vector<unsigned int>& Indices);

	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	bool normalize;

	GLuint m_VAO;
	GLuint m_Buffers[7];

	struct MeshEntry {
		MeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
	};

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture2D *> m_Textures;
	std::vector<MaterialColor> m_colors;
};

#endif //_MESH_H