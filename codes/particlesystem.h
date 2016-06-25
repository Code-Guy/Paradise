#ifndef _PARTICLESYSTEM_H
#define	_PARTICLESYSTEM_H

#include "billboardshader.h"
#include "camera.h"
#include "texture2d.h"
#include <list>

struct Particle
{
	glm::vec3 pos;//位置
	glm::vec3 vec;//速度
	float size;//大小
	float angle;//旋转角度
	float life;//寿命
	float curTime;//当前时间
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

	std::vector<glm::vec3> poss;//粒子位置
	std::vector<glm::vec2> sizeAngles;//粒子的大小和角度

	unsigned int VB_ID;
	unsigned int Size_Angle_B_ID;

	glm::vec3 position;//粒子系统的中心位置
	int minNum, maxNum;
	glm::vec3 ellipsoid;
	float minEmitterRange, maxEmitterRange;
	float minSize, maxSize;
	float minAngle, maxAngle;
	float minLife, maxLife;
	glm::vec3 minVec, maxVec;

	glm::vec3 gravity;//重力

	bool isEnd;//是否粒子系统播放结束

	void InitParticles();//初始化粒子系统的各种属性值
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