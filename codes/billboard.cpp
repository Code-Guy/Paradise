#include "billboard.h"
#include "asset.h"

Billboard::Billboard()
{
	camera = Asset::GetInstance()->GetCamera();

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
}

Billboard::~Billboard()
{
	glDeleteBuffers(1, &VB_ID);
	glDeleteBuffers(1, &Size_Angle_B_ID);
}

void Billboard::SetPos(glm::vec3 pos, glm::vec2 sizeAngle)
{
	this->pos = pos;
	this->sizeAngle =sizeAngle;
}

void Billboard::SetTexture(Texture2D *texture)
{
	this->texture = texture;
}

void Billboard::Render()
{
	//填充数据，绘制
	glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &pos, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Size_Angle_B_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), &sizeAngle, GL_DYNAMIC_DRAW);

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
	glDrawArrays(GL_POINTS, 0, 1);

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}