#ifndef _HANDBLOCK_H
#define	_HANDBLOCK_H

#include "camera.h"
#include "mesh.h"
#include "texture2d.h"
#include "simplelightshader.h"

#include <QMouseEvent>

#define ANIMATION_SPEED 8

#define BEGIN_ROTATION 0
#define END_ROTATION 60

#define BEGIN_OFFSET glm::vec3(5.0f, -3.0f, -6.0f)
#define END_OFFSET glm::vec3(2.0f, -1.0f, -6.0f)

enum AnimateType
{
	DropOut, DropIn, No
};

class HandBlock
{
public:
	HandBlock();
	~HandBlock();

	void Logic(float deltaTime);
	void Render();

	void MousePressEvent(QMouseEvent *evt);

	void SetMesh(Mesh *mesh);
	void SetTexture(Texture2D *texture);
	Texture2D *GetTexture();
	Mesh *GetMesh();

	void Hit();

private:
	const Camera *camera;
	Mesh *mesh;
	Texture2D *texture;

	Mesh *axis;//坐标轴模型

	//用于绘制的矩阵
	glm::mat4 WVP;
	glm::mat4 LightWVP;
	glm::mat4 M;

	float angle;
	glm::vec3 offset;

	AnimateType animateType;

	SimpleLightShader *simpleLightShader;

	/*三种灯光*/
	DirectionalLight dl;
	PointLight pl[SimpleLightShader::MAX_POINT_LIGHTS];
	SpotLight sl[SimpleLightShader::MAX_SPOT_LIGHTS];
};

#endif //_HANDBLOCK_H
