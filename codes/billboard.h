#ifndef _BILLBOARD_H
#define	_BILLBOARD_H

#include "billboardshader.h"
#include "camera.h"
#include "texture2d.h"

#include <vector>

class Billboard
{
public:
	Billboard();
	~Billboard();

	void SetPos(glm::vec3 pos, glm::vec2 sizeAngle = glm::vec2(2.0f, 0.0f));
	void SetTexture(Texture2D *texture);

	void Render();

private:
	const Camera *camera;
	Texture2D *texture;

	BillboardShader *billboardShader;

	glm::vec3 pos;//粒子位置
	glm::vec2 sizeAngle;//粒子的大小和角度

	unsigned int VB_ID;
	unsigned int Size_Angle_B_ID;
};

#endif //_BILLBOARD_H