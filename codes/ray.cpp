#include "ray.h"
#include <GL/glew.h>

using namespace std;

Ray::Ray(const glm::vec3 &p, const glm::vec3 &dir)
{
	this->p = p;
	this->dir = glm::normalize(dir);
}

Ray::~Ray()
{

}

bool Ray::IsIntersectWithCube(glm::ivec3 center) const//和以center为中心的标准立方体求交
{
	glm::vec2 pZ1 = XYPlaneJoint(center.z - 1.0f);
	glm::vec2 pZ2 = XYPlaneJoint(center.z + 1.0f);

	glm::vec2 pY1 = XZPlaneJoint(center.y - 1.0f);
	glm::vec2 pY2 = XZPlaneJoint(center.y + 1.0f);

	glm::vec2 pX1 = YZPlaneJoint(center.x - 1.0f);
	glm::vec2 pX2 = YZPlaneJoint(center.x + 1.0f);

	if ((pZ1 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pZ1, center.x - 1.0f, center.x + 1.0f, center.y - 1.0f, center.y + 1.0f)) ||
		(pZ2 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pZ2, center.x - 1.0f, center.x + 1.0f, center.y - 1.0f, center.y + 1.0f)))
		return true;
	if ((pY1 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pY1, center.x - 1.0f, center.x + 1.0f, center.z - 1.0f, center.z + 1.0f)) ||
		(pY2 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pY2, center.x - 1.0f, center.x + 1.0f, center.z - 1.0f, center.z + 1.0f)))
		return true;
	if ((pX1 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pX1, center.y - 1.0f, center.y + 1.0f, center.z - 1.0f, center.z + 1.0f)) ||
		(pX2 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pX2, center.y - 1.0f, center.y + 1.0f, center.z - 1.0f, center.z + 1.0f)))
		return true;

	return false;
}

float Ray::ProjDisToCube(glm::ivec3 center) const//射线起点到立方体中心的向量到射线向量的投影距离
{
	float dis = glm::dot(glm::vec3(center.x, center.y, center.z) - this->p,  this->dir);
	return dis > 0 ? dis : 1e6;
}

glm::ivec3 Ray::GetPickedOuterCellCenter(glm::ivec3 center)//得到拾取到的cell的外临界cell
{
	glm::vec2 pZ1 = XYPlaneJoint(center.z - 1.0f);
	glm::vec2 pZ2 = XYPlaneJoint(center.z + 1.0f);

	glm::vec2 pY1 = XZPlaneJoint(center.y - 1.0f);
	glm::vec2 pY2 = XZPlaneJoint(center.y + 1.0f);

	glm::vec2 pX1 = YZPlaneJoint(center.x - 1.0f);
	glm::vec2 pX2 = YZPlaneJoint(center.x + 1.0f);

	glm::ivec3 outerCellCenters[6];
	float outerCellCenterDotDiss[6];
	for (int i=0; i<6; i++)
	{
		outerCellCenters[i] = glm::ivec3(1e6, 1e6, 1e6);
	}

	if (pZ1 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pZ1, center.x - 1.0f, center.x + 1.0f, center.y - 1.0f, center.y + 1.0f))
	{
		outerCellCenters[0] = center + glm::ivec3(0, 0, -2);
	}
	if(pZ2 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pZ2, center.x - 1.0f, center.x + 1.0f, center.y - 1.0f, center.y + 1.0f))
	{
		outerCellCenters[1] = center + glm::ivec3(0, 0, 2);
	}
	if (pY1 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pY1, center.x - 1.0f, center.x + 1.0f, center.z - 1.0f, center.z + 1.0f))
	{
		outerCellCenters[2] = center + glm::ivec3(0, -2, 0);
	}
	if(pY2 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pY2, center.x - 1.0f, center.x + 1.0f, center.z - 1.0f, center.z + 1.0f))
	{
		outerCellCenters[3] = center + glm::ivec3(0, 2, 0);
	}
	if (pX1 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pX1, center.y - 1.0f, center.y + 1.0f, center.z - 1.0f, center.z + 1.0f))
	{
		outerCellCenters[4] = center + glm::ivec3(-2, 0, 0);
	}
	if(pX2 != glm::vec2(0.0f, 0.0f) && IsJointOnSide(pX2, center.y - 1.0f, center.y + 1.0f, center.z - 1.0f, center.z + 1.0f))
	{
		outerCellCenters[5] = center + glm::ivec3(2, 0, 0);
	}

	for (int i=0; i<6; i++)
	{
		float dotDis = glm::dot(glm::vec3(outerCellCenters[i]) - this->p, this->dir);
		outerCellCenterDotDiss[i] = dotDis > 0 ? dotDis : 1e6;
	}

	float minOuterCellDotDis = 1e6;
	int minOuterCellDotDisIndex = 0;
	for(int i=0; i<6; i++)
	{
		if (outerCellCenterDotDiss[i] < minOuterCellDotDis)
		{
			minOuterCellDotDis = outerCellCenterDotDiss[i];
			minOuterCellDotDisIndex = i;
		}
	}

	return outerCellCenters[minOuterCellDotDisIndex];
}

Ray CalcPickRay(const Camera *camera, glm::ivec4 viewPort, int x, int y)//计算射线
{
	GLdouble mvMatrix[16];
	GLdouble projMatrix[16];

	GLdouble wX, wY, wZ;

	//模型视图矩阵 投影矩阵
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mvMatrix[4 * i + j] = camera->V[i][j];
			projMatrix[4 * i + j] = camera->P[i][j];
		}
	}

	int rectifyY = viewPort[3] - y - 1;//-1??

	//映射
	gluUnProject(x, rectifyY, 1.0, mvMatrix, projMatrix, glm::value_ptr(viewPort), &wX, &wY, &wZ);

	glm::vec3 p = camera->GetPosition();
	glm::vec3 w = glm::vec3(wX, wY, wZ);

	glm::vec3 dir = glm::normalize(w - p);

	return Ray(p, dir);
}

//光线与XY平面 XZ平面 YZ平面
glm::vec2 Ray::XYPlaneJoint(float z) const
{
	float t = (z - this->p.z) / this->dir.z;

	if (t < 0)
		return glm::vec2(0.0f, 0.0f);

	return glm::vec2(this->p.x + t * this->dir.x, this->p.y + t * this->dir.y);
}

glm::vec2 Ray::XZPlaneJoint(float y) const
{
	float t = (y - this->p.y) / this->dir.y;

	if (t < 0)
		return glm::vec2(0.0f, 0.0f);

	return glm::vec2(this->p.x + t * this->dir.x, this->p.z + t * this->dir.z);
}

glm::vec2 Ray::YZPlaneJoint(float x) const
{
	float t = (x - this->p.x) / this->dir.x;

	if (t < 0)
		return glm::vec2(0.0f, 0.0f);

	return glm::vec2(this->p.y + t * this->dir.y, this->p.z + t * this->dir.z);
}

bool Ray::IsJointOnSide(glm::vec2 joint, float min1, float max1, float min2, float max2, 
	float sideOffsetRatio1, float sideOffsetRatio2) const//判断交点是否在面上
{
	float sideOffset1 = (max1 - min1) * sideOffsetRatio1;
	float sideOffset2 = (max2 - min2) * sideOffsetRatio2;

	return joint.x > min1 + sideOffsetRatio1 && joint.x < max1 - sideOffsetRatio1 && 
		joint.y > min2 + sideOffsetRatio2 && joint.y < max2 - sideOffsetRatio2;
}