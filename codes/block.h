#ifndef _BLOCK_H
#define _BLOCK_H

#include "mesh.h"
#include "camera.h"
#include "ray.h"
#include "shader.h"
#include <list>
#include <string>

#define BLOCK_SIZE_SCALE 0.99f

class Cell
{
public:
	Cell(glm::ivec3 position = MAX_POS, 
		Mesh *mesh = NULL,
		Texture2D *texture = NULL);

	bool operator<(const Cell &rhs);
	glm::vec3 GetVec3Position() const;
	float Depth() const;//cell���ĵ�������ľ���

	glm::ivec3 position;//ש���λ��
	bool rendering;//�Ƿ���Ҫ��Ⱦ
	static const Camera *camera;

	Mesh *mesh;
	Texture2D *texture;
};

//�����Ľ���ש��
class Block
{
public:
	Block(Mesh *mesh, Texture2D *texture);
	Block(std::string meshPath, std::string texturePath);

	~Block();

	void SetLightVP(glm::mat4 lightVP);

	void Logic();//����

	void AddCell(glm::ivec3 position);
	void AddCell(const Cell &cell);
	void DeleteCell(glm::ivec3 position);
	void DeleteCell(const Cell &cell);

	bool ContainCell(glm::ivec3 position);

	void ShadowMapPass(Shader *shader) const;//��ӰͶ������
	void RenderPass(Shader *shader) const;//��Ⱦ����

	Cell PickCell(const Ray &ray) const;//���ѡȡ�����cell

	bool Determine(const Mesh *mesh, const Texture2D *texture) const;//�Ƿ������mesh/texture��

	Texture2D *GetTexture() const;
	Mesh *GetMesh() const;

	bool Empty();

	void SortByDepth();//����cell��������ľ����cells��������

	std::list<Cell> cells;
	std::string meshPath;
	std::string texturePath;

private:
	const Camera *camera;
	Mesh *mesh;
	Texture2D *texture;

	int renderingCellNum;//��Ҫ���Ƶĸ��Ӹ���

	//�ƹ����
	glm::mat4 lightVP;

	//���ڻ��Ƶľ���
	glm::mat4 *WVP;
	glm::mat4 *LightWVP;
	glm::mat4 *M;

	//����shadowmap�ľ���
	glm::mat4 *SWVP;
	glm::mat4 *SLightWVP;
	glm::mat4 *SM;

	void ResetMatrices();//���þ���
};

#endif //_BLOCK_H