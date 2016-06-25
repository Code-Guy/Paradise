#ifndef _RAY_H
#define _RAY_H

#include <glm/glm.hpp>
#include "camera.h"

class Ray
{
public:
	Ray(const glm::vec3 &p = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &dir = glm::vec3(0.0f, 0.0f, -1.0f));
	~Ray();

	glm::vec3 p;//Դ��
	glm::vec3 dir;//�����ѹ�һ����

	bool IsIntersectWithCube(glm::ivec3 center) const;//����centerΪ���ĵı�׼��������
	float ProjDisToCube(glm::ivec3 center) const;//������㵽���������ĵ�����������������ͶӰ����
	glm::ivec3 GetPickedOuterCellCenter(glm::ivec3 center);//�õ�ʰȡ����cell�����ٽ�cell

	friend Ray CalcPickRay(const Camera *camera, glm::ivec4 viewPort, int x, int y);//��������

private:
	//������XYƽ�� XZƽ�� YZƽ��
	glm::vec2 XYPlaneJoint(float z) const;
	glm::vec2 XZPlaneJoint(float y) const;
	glm::vec2 YZPlaneJoint(float x) const;

	bool IsJointOnSide(glm::vec2 joint, float min1, float max1, float min2, float max2, 
		float sideOffsetRatio1 = 0, float sideOffsetRatio2 = 0) const;//�жϽ����Ƿ�������
};

#endif //_RAY_H