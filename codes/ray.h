#ifndef _RAY_H
#define _RAY_H

#include <glm/glm.hpp>
#include "camera.h"

class Ray
{
public:
	Ray(const glm::vec3 &p = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &dir = glm::vec3(0.0f, 0.0f, -1.0f));
	~Ray();

	glm::vec3 p;//源点
	glm::vec3 dir;//方向（已归一化）

	bool IsIntersectWithCube(glm::ivec3 center) const;//和以center为中心的标准立方体求交
	float ProjDisToCube(glm::ivec3 center) const;//射线起点到立方体中心的向量到射线向量的投影距离
	glm::ivec3 GetPickedOuterCellCenter(glm::ivec3 center);//得到拾取到的cell的外临界cell

	friend Ray CalcPickRay(const Camera *camera, glm::ivec4 viewPort, int x, int y);//计算射线

private:
	//光线与XY平面 XZ平面 YZ平面
	glm::vec2 XYPlaneJoint(float z) const;
	glm::vec2 XZPlaneJoint(float y) const;
	glm::vec2 YZPlaneJoint(float x) const;

	bool IsJointOnSide(glm::vec2 joint, float min1, float max1, float min2, float max2, 
		float sideOffsetRatio1 = 0, float sideOffsetRatio2 = 0) const;//判断交点是否在面上
};

#endif //_RAY_H