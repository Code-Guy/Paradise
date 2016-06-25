#include "overlay2d.h"
#include <string.h>
#include <vector>
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

Overlay2DShader *Overlay2D::overlay2dShader = NULL;
Texture2D *Overlay2D::textTex = NULL;

unsigned int Overlay2D::VB_ID = INVALID_VALUE;
unsigned int Overlay2D::UVB_ID = INVALID_VALUE;

glm::vec2 Overlay2D::ratio = glm::vec2(1.0f, 1.0f);
glm::vec2 Overlay2D::screenSize(WIDTH, HEIGHT);

bool Overlay2D::Init(string textTexFileName)
{
	textTex = new Texture2D(textTexFileName);
	overlay2dShader = new Overlay2DShader;

	if (!overlay2dShader->Init()) {
		printf("Error initializing the overlay2dShader\n");
		return false;
	}
	overlay2dShader->Enable();
	overlay2dShader->SetTextureUnit(0);

	glGenBuffers(1, &VB_ID);
	glGenBuffers(1, &UVB_ID);
	
	return true;
}

void Overlay2D::Print(string text, int x, int y, int size, glm::vec4 color, float spanRatio)
{
	unsigned int len = text.length();

	vector<glm::vec2> vertices;
	vector<glm::vec2> uvs;

	//fill the buffers
	for(int i=0; i<len; i++)
	{
		glm::vec2 vertex_up_left    = glm::vec2( x+i*(1 - spanRatio) * size, y+size) * ratio;
		glm::vec2 vertex_up_right   = glm::vec2( x+i*(1 - spanRatio) * size+size, y+size) * ratio;
		glm::vec2 vertex_down_right = glm::vec2( x+i*(1 - spanRatio) * size+size, y) * ratio;
		glm::vec2 vertex_down_left  = glm::vec2( x+i*(1 - spanRatio) * size, y) * ratio;

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16) / 16.0f;
		float uv_y = (character/16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2( uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2( uv_x+1.0f/16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f));
		glm::vec2 uv_down_left = glm::vec2( uv_x, (uv_y + 1.0f/16.0f));

		uvs.push_back(uv_up_left);
		uvs.push_back(uv_down_left);
		uvs.push_back(uv_up_right);

		uvs.push_back(uv_down_right);
		uvs.push_back(uv_up_right);
		uvs.push_back(uv_down_left);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, UVB_ID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	overlay2dShader->Enable();
	overlay2dShader->SetColor(color);

	textTex->Bind(GL_TEXTURE0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVB_ID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Overlay2D::Draw(Texture2D *tex, int x, int y, glm::vec2 sizeRatio, glm::vec4 color)
{
	vector<glm::vec2> vertices;
	vector<glm::vec2> uvs;

	//fill the buffers
	glm::vec2 vertex_up_left    = glm::vec2(x, y + sizeRatio.y * tex->height) * ratio;
	glm::vec2 vertex_up_right   = glm::vec2(x + sizeRatio.x * tex->width, y + sizeRatio.y * tex->height) * ratio;
	glm::vec2 vertex_down_right = glm::vec2(x + sizeRatio.x * tex->width, y) * ratio;
	glm::vec2 vertex_down_left  = glm::vec2(x, y) * ratio;

	vertices.push_back(vertex_up_left);
	vertices.push_back(vertex_down_left);
	vertices.push_back(vertex_up_right);

	vertices.push_back(vertex_down_right);
	vertices.push_back(vertex_up_right);
	vertices.push_back(vertex_down_left);

	glm::vec2 uv_up_left = glm::vec2(0, 0);
	glm::vec2 uv_up_right = glm::vec2(1, 0);
	glm::vec2 uv_down_right = glm::vec2(1, 1);
	glm::vec2 uv_down_left = glm::vec2(0, 1);

	uvs.push_back(uv_up_left);
	uvs.push_back(uv_down_left);
	uvs.push_back(uv_up_right);

	uvs.push_back(uv_down_right);
	uvs.push_back(uv_up_right);
	uvs.push_back(uv_down_left);

	glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, UVB_ID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	overlay2dShader->Enable();
	overlay2dShader->SetColor(color);

	tex->Bind(GL_TEXTURE0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVB_ID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Overlay2D::Draw(Texture2D *tex, int x, int y, int width, int height, glm::vec4 color)
{
	vector<glm::vec2> vertices;
	vector<glm::vec2> uvs;

	//fill the buffers
	glm::vec2 vertex_up_left    = glm::vec2(x, y + height) * ratio;
	glm::vec2 vertex_up_right   = glm::vec2(x + width, y + height) * ratio;
	glm::vec2 vertex_down_right = glm::vec2(x + width, y) * ratio;
	glm::vec2 vertex_down_left  = glm::vec2(x, y) * ratio;

	vertices.push_back(vertex_up_left);
	vertices.push_back(vertex_down_left);
	vertices.push_back(vertex_up_right);

	vertices.push_back(vertex_down_right);
	vertices.push_back(vertex_up_right);
	vertices.push_back(vertex_down_left);

	glm::vec2 uv_up_left = glm::vec2(0, 0);
	glm::vec2 uv_up_right = glm::vec2(1, 0);
	glm::vec2 uv_down_right = glm::vec2(1, 1);
	glm::vec2 uv_down_left = glm::vec2(0, 1);

	uvs.push_back(uv_up_left);
	uvs.push_back(uv_down_left);
	uvs.push_back(uv_up_right);

	uvs.push_back(uv_down_right);
	uvs.push_back(uv_up_right);
	uvs.push_back(uv_down_left);

	glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, UVB_ID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	overlay2dShader->Enable();
	overlay2dShader->SetColor(color);

	tex->Bind(GL_TEXTURE0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVB_ID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Overlay2D::Release()
{
	if (VB_ID != INVALID_VALUE)
	{
		glDeleteBuffers(1, &VB_ID);
	}
	if (UVB_ID != INVALID_VALUE)
	{
		glDeleteBuffers(1, &UVB_ID);
	}

	SAFE_DELETE(overlay2dShader);
	SAFE_DELETE(textTex);
}

void Overlay2D::Resize(int width, int height)
{
	screenSize = glm::vec2(width, height);
	overlay2dShader->Enable();
	overlay2dShader->SetScreenSize(width, height);
	ratio = glm::vec2(1.0f * width / WIDTH, 1.0f * height / HEIGHT);
}