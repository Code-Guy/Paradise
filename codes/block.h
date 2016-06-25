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
	float Depth() const;//cell中心到摄像机的距离

	glm::ivec3 position;//砖块的位置
	bool rendering;//是否需要渲染
	static const Camera *camera;

	Mesh *mesh;
	Texture2D *texture;
};

//基础的建筑砖块
class Block
{
public:
	Block(Mesh *mesh, Texture2D *texture);
	Block(std::string meshPath, std::string texturePath);

	~Block();

	void SetLightVP(glm::mat4 lightVP);

	void Logic();//更新

	void AddCell(glm::ivec3 position);
	void AddCell(const Cell &cell);
	void DeleteCell(glm::ivec3 position);
	void DeleteCell(const Cell &cell);

	bool ContainCell(glm::ivec3 position);

	void ShadowMapPass(Shader *shader) const;//阴影投射批次
	void RenderPass(Shader *shader) const;//渲染批次

	Cell PickCell(const Ray &ray) const;//鼠标选取最近的cell

	bool Determine(const Mesh *mesh, const Texture2D *texture) const;//是否是这个mesh/texture对

	Texture2D *GetTexture() const;
	Mesh *GetMesh() const;

	bool Empty();

	void SortByDepth();//根据cell到摄像机的距离对cells进行排序

	std::list<Cell> cells;
	std::string meshPath;
	std::string texturePath;

private:
	const Camera *camera;
	Mesh *mesh;
	Texture2D *texture;

	int renderingCellNum;//需要绘制的格子个数

	//灯光矩阵
	glm::mat4 lightVP;

	//用于绘制的矩阵
	glm::mat4 *WVP;
	glm::mat4 *LightWVP;
	glm::mat4 *M;

	//用于shadowmap的矩阵
	glm::mat4 *SWVP;
	glm::mat4 *SLightWVP;
	glm::mat4 *SM;

	void ResetMatrices();//重置矩阵
};

#endif //_BLOCK_H