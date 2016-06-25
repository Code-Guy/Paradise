#include "particlesystem.h"
#include "global.h"
#include "tool.h"
#include "asset.h"
#include <vector>

using namespace std;

ParticleSystem::ParticleSystem(Texture2D *texture,
	glm::vec3 position,
	int minNum, int maxNum,
	glm::vec3 ellipsoid,
	float minEmitterRange, float maxEmitterRange,
	float minSize, float maxSize,
	float minAngle, float maxAngle,
	float minLife, float maxLife,
	glm::vec3 minVec, glm::vec3 maxVec)
	: texture(texture),
	position(position),
	minNum(minNum), maxNum(maxNum),
	ellipsoid(ellipsoid),
	minEmitterRange(minEmitterRange), maxEmitterRange(maxEmitterRange),
	minSize(minSize), maxSize(maxSize),
	minAngle(minAngle), maxAngle(maxAngle),
	minLife(minLife), maxLife(maxLife),
	minVec(minVec), maxVec(maxVec)
{
	camera = Asset::GetInstance()->GetCamera();
	isEnd = false;
	gravity = glm::vec3(0, -20.0f, 0);
	//创建点buffer
	glGenBuffers(1, &VB_ID);
	glGenBuffers(1, &Size_Angle_B_ID);

	billboardShader = new BillboardShader;
	//初始化billboard shader
	if (!billboardShader->Init()) {
		printf("Error initializing the billboardShader\n");
	}
	billboardShader->Enable();
	billboardShader->SetTextureUnit(0);

	InitParticles();//初始化粒子系统的各种属性值
}

ParticleSystem::~ParticleSystem()
{
	glDeleteBuffers(1, &VB_ID);
	glDeleteBuffers(1, &Size_Angle_B_ID);
}

void ParticleSystem::Logic(float deltaTime)
{
	if (!isEnd)
	{
		poss.clear();
		sizeAngles.clear();

		//先更新所有粒子的属性值
		for (list<Particle>::iterator pi = particles.begin(); pi != particles.end(); )
		{
			pi->curTime += deltaTime;
			if (pi->curTime > pi->life)
			{
				pi = particles.erase(pi);
				continue;
			}

			glm::vec3 newVec = pi->vec + gravity * deltaTime;//计算新速度
			pi->pos += (pi->vec + newVec) * 0.5f * deltaTime;//更新粒子位置
			pi->vec = newVec;//更新粒子速度

			poss.push_back(pi->pos);
			sizeAngles.push_back(glm::vec2(pi->size, pi->angle));
			pi++;
		}

		if (particles.empty())
		{
			isEnd = true;
			return;
		}
	}
}

void ParticleSystem::Render()
{
	if (!isEnd)
	{
		//填充数据，绘制
		glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
		glBufferData(GL_ARRAY_BUFFER, poss.size() * sizeof(glm::vec3), &poss[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, Size_Angle_B_ID);
		glBufferData(GL_ARRAY_BUFFER, sizeAngles.size() * sizeof(glm::vec2), &sizeAngles[0], GL_DYNAMIC_DRAW);

		billboardShader->Enable();
		billboardShader->SetMV(camera->V);
		billboardShader->SetP(camera->P);
		texture->Bind(GL_TEXTURE0);

		// 1rst attribute buffer : pos
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2rst attribute buffer : size_angle
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Size_Angle_B_ID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Draw call
		glDrawArrays(GL_POINTS, 0, poss.size());

		glDisable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

bool ParticleSystem::IsEnd()
{
	return isEnd;
}

void ParticleSystem::InitParticles()//初始化粒子系统的各种属性值
{
	int num = RandIntToInt(minNum, maxNum);
	float emitterRange = RandFloatToFloat(minEmitterRange, maxEmitterRange);

	for (int i=0; i<num; i++)
	{
		Particle p;
		p.pos = UniformEllipsoidSample(ellipsoid, emitterRange) + position;
		p.size = RandFloatToFloat(minSize, maxSize);
		p.angle = RandFloatToFloat(minAngle, maxAngle);
		p.life = RandFloatToFloat(minLife, maxLife);
		p.vec = RandVec3ToVec3(minVec, maxVec);
		p.curTime = 0;

		particles.push_back(p);
	}
}

PSEffect::PSEffect()
{

}

PSEffect::~PSEffect()
{
	for (list<ParticleSystem *>::iterator psi = pss.begin(); psi != pss.end(); psi++)
	{
		delete *psi;
	}
	pss.clear();
}

void PSEffect::Logic(float deltaTime)
{
	for (list<ParticleSystem *>::iterator psi = pss.begin(); psi != pss.end(); )
	{
		if ((*psi)->IsEnd())
		{
			delete *psi;
			psi = pss.erase(psi);
		}
		else
		{
			(*psi)->Logic(deltaTime);
			psi++;
		}
	}
}

void PSEffect::Render()
{
	for (list<ParticleSystem *>::iterator psi = pss.begin(); psi != pss.end(); psi++)
	{
		(*psi)->Render();
	}
}

void PSEffect::AddPS(ParticleSystem *ps)
{
	pss.push_back(ps);
}