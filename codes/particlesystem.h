#ifndef _PARTICLESYSTEM_H
#define	_PARTICLESYSTEM_H

#include "billboardshader.h"
#include "camera.h"
#include "texture2d.h"
#include <list>

struct Particle
{
	glm::vec3 pos;//λ��
	glm::vec3 vec;//�ٶ�
	float size;//��С
	float angle;//��ת�Ƕ�
	float life;//����
	float curTime;//��ǰʱ��
};

class ParticleSystem
{
public:
	ParticleSystem(Texture2D *texture,
		glm::vec3 position,
		int minNum, int maxNum,
		glm::vec3 ellipsoid,
		float minEmitterRange, float maxEmitterRange,
		float minSize, float maxSize,
		float minAngle, float maxAngle,
		float minLife, float maxLife,
		glm::vec3 minVec, glm::vec3 maxVec);
	~ParticleSystem();

	void Logic(float deltaTime);
	void Render();
	bool IsEnd();

private:
	const Camera *camera;
	Texture2D *texture;

	std::list<Particle> particles;
	BillboardShader *billboardShader;

	std::vector<glm::vec3> poss;//����λ��
	std::vector<glm::vec2> sizeAngles;//���ӵĴ�С�ͽǶ�

	unsigned int VB_ID;
	unsigned int Size_Angle_B_ID;

	glm::vec3 position;//����ϵͳ������λ��
	int minNum, maxNum;
	glm::vec3 ellipsoid;
	float minEmitterRange, maxEmitterRange;
	float minSize, maxSize;
	float minAngle, maxAngle;
	float minLife, maxLife;
	glm::vec3 minVec, maxVec;

	glm::vec3 gravity;//����

	bool isEnd;//�Ƿ�����ϵͳ���Ž���

	void InitParticles();//��ʼ������ϵͳ�ĸ�������ֵ
};

class PSEffect
{
public:
	PSEffect();
	~PSEffect();

	void Logic(float deltaTime);
	void Render();
	void AddPS(ParticleSystem *ps);
	
private:
	std::list<ParticleSystem *> pss;
};

#endif //_PARTICLESYSTEM_H