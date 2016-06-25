#include "primitiverenderer.h"
#include "global.h"
#include "asset.h"
using namespace std;

unsigned int PrimitiveRenderer::VB_ID = INVALID_VALUE;
LineShader *PrimitiveRenderer::lineShader = NULL;
const Camera *PrimitiveRenderer::camera = NULL;

void PrimitiveRenderer::Init()
{
	PrimitiveRenderer::camera = Asset::GetInstance()->GetCamera();
	glGenBuffers(1, &VB_ID);

	//初始化line shader
	lineShader = new LineShader;
	if (!lineShader->Init())
	{
		printf("Error initializing the line shader\n");
	}

	glLineWidth(5.0f);
}

void PrimitiveRenderer::DrawCubeWireFrame(glm::vec3 position, glm::vec4 color)//绘制网格模式的block
{
	vector<glm::vec3> vertices;

	vertices.push_back(position + glm::vec3(-1, 1, -1));
	vertices.push_back(position + glm::vec3(1, 1, -1));
	vertices.push_back(position + glm::vec3(1, 1, -1));
	vertices.push_back(position + glm::vec3(1, 1, 1));
	vertices.push_back(position + glm::vec3(1, 1, 1));
	vertices.push_back(position + glm::vec3(-1, 1, 1));
	vertices.push_back(position + glm::vec3(-1, 1, 1));
	vertices.push_back(position + glm::vec3(-1, 1, -1));

	vertices.push_back(position + glm::vec3(-1, 1, -1));
	vertices.push_back(position + glm::vec3(-1, -1, -1));
	vertices.push_back(position + glm::vec3(1, 1, -1));
	vertices.push_back(position + glm::vec3(1, -1, -1));
	vertices.push_back(position + glm::vec3(1, 1, 1));
	vertices.push_back(position + glm::vec3(1, -1, 1));
	vertices.push_back(position + glm::vec3(-1, 1, 1));
	vertices.push_back(position + glm::vec3(-1, -1, 1));

	vertices.push_back(position + glm::vec3(-1, -1, -1));
	vertices.push_back(position + glm::vec3(1, -1, -1));
	vertices.push_back(position + glm::vec3(1, -1, -1));
	vertices.push_back(position + glm::vec3(1, -1, 1));
	vertices.push_back(position + glm::vec3(1, -1, 1));
	vertices.push_back(position + glm::vec3(-1, -1, 1));
	vertices.push_back(position + glm::vec3(-1, -1, 1));
	vertices.push_back(position + glm::vec3(-1, -1, -1));

	DrawBase(vertices, color);
}

void PrimitiveRenderer::DrawRay(const Ray &ray, glm::vec4 color, float length)//画射线
{
	vector<glm::vec3> vertices;

	vertices.push_back(ray.p);
	vertices.push_back(ray.p + ray.dir * length);

	DrawBase(vertices, color);
}

void PrimitiveRenderer::Release()
{
	if (VB_ID != INVALID_VALUE)
	{
		glDeleteBuffers(1, &VB_ID);
	}

	SAFE_DELETE(lineShader);
}

void PrimitiveRenderer::DrawBase(const std::vector<glm::vec3> &vertices, glm::vec4 color)
{
	glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);

	lineShader->Enable();
	lineShader->SetMVP(camera->VP);
	lineShader->SetLineColor(color);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw call
	glDrawArrays(GL_LINES, 0, vertices.size());

	glDisableVertexAttribArray(0);
}