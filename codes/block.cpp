#include "block.h"
#include "asset.h"

using namespace std;

const Camera *Cell::camera = NULL;

Cell::Cell(glm::ivec3 position, 
	Mesh *mesh,
	Texture2D *texture) : position(position), mesh(mesh), texture(texture)
{
	rendering = true;
}

bool Cell::operator<(const Cell &rhs)
{
	return this->Depth() > rhs.Depth();
}

glm::vec3 Cell::GetVec3Position() const
{
	return glm::vec3(position.x, position.y, position.z);
}

float Cell::Depth() const//cell���ĵ�������ľ���
{
	glm::vec3 cameraToCellVector = GetVec3Position() - camera->GetPosition();
	return glm::dot(cameraToCellVector, camera->GetDirection());
}

Block::Block(Mesh *mesh, Texture2D *texture)
{
	this->camera = Asset::GetInstance()->GetCamera();
	Cell::camera = camera;

	this->mesh = mesh;
	this->texture = texture;

	this->meshPath = Asset::GetInstance()->GetMeshPath(mesh);
	this->texturePath = Asset::GetInstance()->GetTexturePath(texture);

	WVP = NULL;
	LightWVP = NULL;
	M = NULL;
	SWVP = NULL;
	SLightWVP = NULL;
	SM = NULL;

	cells.clear();
}

Block::Block(std::string meshPath, std::string texturePath)
{
	this->meshPath = meshPath;
	this->texturePath = texturePath;

	this->mesh = NULL;
	this->texture = NULL;

	WVP = NULL;
	LightWVP = NULL;
	M = NULL;
	SWVP = NULL;
	SLightWVP = NULL;
	SM = NULL;

	cells.clear();
}

Block::~Block()
{
	cells.clear();
	SAFE_DELETE(WVP);
	SAFE_DELETE(LightWVP);
	SAFE_DELETE(M);
	SAFE_DELETE(SWVP);
	SAFE_DELETE(SLightWVP);
	SAFE_DELETE(SM);
}

void Block::SetLightVP(glm::mat4 lightVP)
{
	this->lightVP = lightVP;
}

void Block::Logic()//����
{
	//������ӵĿɼ���
	for (list<Cell>::iterator ci = cells.begin(); ci != cells.end(); ci++)
	{
		ci->rendering = camera->IsBlockInFrustum(ci->position);
	}

	ResetMatrices();//���þ���
}

void Block::AddCell(glm::ivec3 position)
{
	cells.push_back(Cell(position, mesh, texture));
}

void Block::AddCell(const Cell &cell)
{
	cells.push_back(cell);
}

void Block::DeleteCell(glm::ivec3 position)
{
	for (list<Cell>::const_iterator ci = cells.begin(); ci != cells.end(); ci++)
	{
		if (ci->position == position)
		{
			cells.erase(ci);
			break;
		}
	}
}

void Block::DeleteCell(const Cell &cell)
{
	for (list<Cell>::const_iterator ci = cells.begin(); ci != cells.end(); ci++)
	{
		if (ci->position == cell.position)
		{
			cells.erase(ci);
			break;
		}
	}
}

bool Block::ContainCell(glm::ivec3 position)
{
	for (list<Cell>::const_iterator ci = cells.begin(); ci != cells.end(); ci++)
	{
		if (ci->position == position)
		{
			return true;
		}
	}
	return false;
}

void Block::ShadowMapPass(Shader *shader) const//��ӰͶ������
{
	mesh->Render(shader, cells.size(), SWVP, SLightWVP, SM, texture);
}

void Block::RenderPass(Shader *shader) const//��Ⱦ����
{
	mesh->Render(shader, renderingCellNum, WVP, LightWVP, M, texture);
}

Cell Block::PickCell(const Ray &ray) const//���ѡȡ�����cell
{
	Cell pickedCell;//�����ѡȡ��cell��Ĭ������Զ
	for (list<Cell>::const_iterator ci = cells.begin(); ci != cells.end(); ci++)
	{
		if (ci->rendering && ray.IsIntersectWithCube(ci->position))
		{
			if (ray.ProjDisToCube(ci->position) < ray.ProjDisToCube(pickedCell.position))
			{
				pickedCell = *ci;
			}
		}
	}
	return pickedCell;
}

bool Block::Determine(const Mesh *mesh, const Texture2D *texture) const//�Ƿ������mesh/texture��
{
	return (mesh == this->mesh) && (texture == this->texture);
}

Texture2D *Block::GetTexture() const
{
	return texture;
}

Mesh *Block::GetMesh() const
{
	return mesh;
}

bool Block::Empty()
{
	return cells.empty();
}

void Block::SortByDepth()//����cell��������ľ����cells��������
{
	cells.sort();
}

void Block::ResetMatrices()//���þ���
{
	//����WVP LightWVP M����
	renderingCellNum = 0;
	for (list<Cell>::const_iterator ci = cells.begin(); ci != cells.end(); ci++)
	{
		if (ci->rendering)
		{
			renderingCellNum++;
		}
	}

	SAFE_DELETE(WVP);
	SAFE_DELETE(LightWVP);
	SAFE_DELETE(M);
	SAFE_DELETE(SWVP);
	SAFE_DELETE(SLightWVP);
	SAFE_DELETE(SM);

	WVP = new glm::mat4[renderingCellNum];
	LightWVP = new glm::mat4[renderingCellNum];
	M = new glm::mat4[renderingCellNum];
	SWVP = new glm::mat4[cells.size()];
	SLightWVP = new glm::mat4[cells.size()];
	SM = new glm::mat4[cells.size()];

	int index = 0;
	int sIndex = 0;
	for (list<Cell>::const_iterator ci = cells.begin(); ci != cells.end(); ci++, sIndex++)
	{
		if (ci->rendering)
		{
			//M[index] = glm::translate(ci->GetVec3Position()) * glm::scale(BLOCK_SIZE_SCALE, BLOCK_SIZE_SCALE, BLOCK_SIZE_SCALE);
			M[index] = glm::translate(ci->GetVec3Position());
			LightWVP[index] = lightVP * M[index];
			WVP[index] = camera->VP * M[index];
			index++;
		}

		SM[sIndex] = glm::translate(ci->position);
		SLightWVP[sIndex] = lightVP * SM[sIndex];
		SWVP[sIndex] = camera->VP * SM[sIndex];
	}
}