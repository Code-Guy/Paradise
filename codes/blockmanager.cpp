#include "blockmanager.h"
#include "primitiverenderer.h"
#include "jsonhandler.h"
#include "asset.h"
#include <QDebug>

using namespace std;

BlockManager::BlockManager()
{
	maxPickedCellDis = DEFAULT_MAX_PICKED_CELL_DIS;
	camera = Asset::GetInstance()->GetCamera();

	//设置灯光shader
	lightShader = new LightShader;
	if (!lightShader->Init())
	{
		printf("Error initializing the lighting shader\n");
		return;
	}
	lightShader->Enable();
	lightShader->SetColorTextureUnit(0);
	lightShader->SetShadowMapTextureUnit(1);
	lightShader->SetShadowMapSize(1024, 1024);
	lightShader->Validate();

	//初始化shadowmap shader
	shadowMapShader = new ShadowMapShader;
	if(!shadowMapShader->Init())
	{
		printf("Error initializing the shadow map shader\n");
	}
	//初始化shadowmap fbo
	shaderMapFBO = new ShadowMapFBO;
	if (!shaderMapFBO->Init(1024, 1024))
	{
		printf("Error initializing the shadow map fbo\n");
	}

	psEffect = new PSEffect;
	mediaPlayer = MediaPlayer::GetInstance();
}

BlockManager::~BlockManager()
{
	delete lightShader;
	delete shadowMapShader;
	delete shaderMapFBO;
	delete psEffect;
}

void BlockManager::Logic(float deltaTime)
{
	UpdateLightVP(dl);

	for (list<Block>::iterator bi = blocks.begin(); bi != blocks.end(); bi++)
	{
		bi->SetLightVP(lightVP);
		bi->Logic();
	}

	dlBd.SetPos(glm::vec3(0, 0, 0));
	dlBd.SetTexture(Asset::GetInstance()->GetTexture("Resources/texture/directional_light.png", GL_LINEAR, GL_LINEAR));

	plBds.clear();
	for (vector<PointLight>::const_iterator pli = pls.begin(); pli != pls.end(); pli++)
	{
		Billboard plBd;
		plBd.SetPos(pli->Position);
		plBd.SetTexture(Asset::GetInstance()->GetTexture("Resources/texture/point_light.png", GL_LINEAR, GL_LINEAR));
		plBds.push_back(plBd);
	}

	slBds.clear();
	for (vector<SpotLight>::const_iterator sli = sls.begin(); sli != sls.end(); sli++)
	{
		Billboard slBd;
		slBd.SetPos(sli->Position);
		slBd.SetTexture(Asset::GetInstance()->GetTexture("Resources/texture/spot_light.png", GL_LINEAR, GL_LINEAR));
		slBds.push_back(slBd);
	}

	PickCell();
	psEffect->Logic(deltaTime);
}

void BlockManager::Render()
{
	//shadowmap pass
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	shaderMapFBO->BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);

	shadowMapShader->Enable();

	for (list<Block>::const_iterator bi = blocks.begin(); bi != blocks.end(); bi++)
	{
		bi->ShadowMapPass(shadowMapShader);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//render pass
	//glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);//用背景色清除颜色缓冲

	lightShader->Enable();
	lightShader->SetEyeWorldPos(camera->GetPosition());
	lightShader->SetDirectionalLight(dl);
	lightShader->SetPointLights(pls);
	lightShader->SetSpotLights(sls);

	shaderMapFBO->BindForReading(GL_TEXTURE1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (list<Block>::iterator bi = blocks.begin(); bi != blocks.end(); bi++)
	{
		bi->SortByDepth();
		bi->RenderPass(lightShader);
	}

	if (pickedCell.position != MAX_POS)
	{
		PrimitiveRenderer::DrawCubeWireFrame(pickedCell.GetVec3Position());
	}

	psEffect->Render();

	glClear(GL_DEPTH_BUFFER_BIT);//用背景色清除颜色缓冲
	dlBd.Render();
	for (int i=0; i<plBds.size(); i++)
	{
		plBds[i].Render();
	}
	for (int i=0; i<slBds.size(); i++)
	{
		slBds[i].Render();
	}

	glDisable(GL_BLEND);
}

void BlockManager::Resize(int width, int height)
{
	lightShader->Enable();
	lightShader->SetShadowMapSize(width, height);
	shaderMapFBO->ResizeTexture(width, height);

	this->width = width;
	this->height = height;
}

void BlockManager::MousePressEvent(QMouseEvent *evt, Mesh *mesh, Texture2D *texture)
{
	if (pickedCell.position != MAX_POS)
	{
		switch(evt->button())
		{
		case Qt::LeftButton:
			{
				mediaPlayer->PlayAudio(PutCell);
				glm::ivec3 outerCellCenter = pickRay.GetPickedOuterCellCenter(pickedCell.position);
				AddCellToBlock(outerCellCenter, mesh, texture);

				if (ctrlzCells.size() < MAX_HISTORY_NUM)
				{
					vector<Cell> historyCells;
					historyCells.push_back(Cell(outerCellCenter, mesh, texture));
					ctrlzCells.push_back(make_pair(true, historyCells));
				}
			}
				break;
		case Qt::RightButton:
			{
				//if (!RestLastCell())
				{
					mediaPlayer->PlayAudio(DeleteCell);
					Mesh *deletedCellMesh = NULL;
					Texture2D *deletedCellTexture = NULL;
					DeleteCellInBlock(pickedCell.position, deletedCellMesh, deletedCellTexture);

					ParticleSystem *ps = new ParticleSystem(deletedCellTexture, 
						pickedCell.GetVec3Position(),
						20, 25,
						glm::vec3(1, 1, 1),
						0.8, 1,
						0.1, 0.2,
						0, 60,
						0.5, 0.8,
						glm::vec3(-2, 3, -2), glm::vec3(2, 4, 2));

					psEffect->AddPS(ps);

					if (ctrlzCells.size() < MAX_HISTORY_NUM)
					{
						vector<Cell> historyCells;
						historyCells.push_back(Cell(pickedCell.position, deletedCellMesh, deletedCellTexture));
						ctrlzCells.push_back(make_pair(false, historyCells));
					}
				}
				break;
			}
		}
		pickedCell.position = MAX_POS;
	}
}

void BlockManager::MouseMoveEvent(QMouseEvent *evt)
{
	lastMousePos = glm::ivec2(evt->pos().x(), evt->pos().y());
}

void BlockManager::KeyPressEvent(QKeyEvent *evt)
{
	bool op;
	vector<Cell> historyCells;

	if (evt->modifiers() == Qt::ControlModifier && evt->key() == Qt::Key_Z && !ctrlzCells.empty())
	{
		op = ctrlzCells.back().first;
		historyCells = ctrlzCells.back().second;

		ctrlyCells.push_back(make_pair(!op, historyCells));
		ctrlzCells.pop_back();
	}
	else if (evt->modifiers() == Qt::ControlModifier && evt->key() == Qt::Key_Y && !ctrlyCells.empty())
	{
		op = ctrlyCells.back().first;
		historyCells = ctrlyCells.back().second;

		ctrlzCells.push_back(make_pair(!op, historyCells));
		ctrlyCells.pop_back();
	}

	if (!historyCells.empty())
	{
		if (op)
		{
			DeleteBulkCellToBlock(historyCells);
		}
		else
		{
			AddBulkCellToBlock(historyCells);
		}
	}
}

glm::ivec3 BlockManager::GetPickedCellPos()
{
	return pickedCell.position;
}

void BlockManager::SetMaxPickedCellDis(float maxPickedCellDis)
{
	this->maxPickedCellDis = maxPickedCellDis;
}

void BlockManager::OpenWorld(QString worldID, vector<QString> &shortcutMeshPaths, vector<QString> &shortcutTexPaths)
{
	this->worldID = worldID;
	//清空历史记录
	ctrlzCells.clear();
	ctrlyCells.clear();
	//载入block json文件
	JsonHandler::ReadBlocks(worldID, 
		blocks, 
		shortcutMeshPaths, shortcutTexPaths,
		dl, pls, sls);
}

void BlockManager::SaveWorld(const vector<QString> &shortcutMeshPaths, const vector<QString> &shortcutTexPaths)
{
	JsonHandler::WriteBlocks(worldID, 
		blocks, 
		shortcutMeshPaths, shortcutTexPaths,
		dl, pls, sls);
}

bool BlockManager::OverlapTest(glm::ivec3 startPoint, glm::ivec3 endPoint)
{
	if (startPoint.x > endPoint.x || 
		startPoint.y > endPoint.y || 
		startPoint.z > endPoint.z)
	{
		return true;
	}

	startPoint = startPoint * 2;
	endPoint = endPoint * 2;

	for (list<Block>::const_iterator bi = blocks.begin(); bi != blocks.end(); bi++)
	{
		for (list<Cell>::const_iterator ci = bi->cells.begin(); ci != bi->cells.end(); ci++)
		{
			if (ci->position.x >= startPoint.x && ci->position.x <= endPoint.x &&
				ci->position.y >= startPoint.y && ci->position.y <= endPoint.y &&
				ci->position.z >= startPoint.z && ci->position.z <= endPoint.z)
			{
				return true;
			}
		}
	}

	return false;
}

void BlockManager::AddBatchCellToBlock(glm::ivec3 startPoint, glm::ivec3 endPoint, Mesh *mesh, Texture2D *texture)
{
	list<Block>::iterator bi;

	for (bi = blocks.begin(); bi != blocks.end(); bi++)
	{
		if (bi->Determine(mesh, texture))
		{
			break;
		}
	}

	if (bi != blocks.end())
	{
		for (int i=startPoint.x; i<=endPoint.x; i++)
		{
			for (int j=startPoint.y; j<=endPoint.y; j++)
			{
				for (int k=startPoint.z; k<=endPoint.z; k++)
				{
					bi->AddCell(glm::ivec3(i * 2, j * 2, k * 2));
				}
			}
		}
	}
	else
	{
		Block block(mesh, texture);

		for (int i=startPoint.x; i<=endPoint.x; i++)
		{
			for (int j=startPoint.y; j<=endPoint.y; j++)
			{
				for (int k=startPoint.z; k<=endPoint.z; k++)
				{
					block.AddCell(glm::ivec3(i * 2, j * 2, k * 2));
				}
			}
		}
		blocks.push_back(block);
	}

	if (ctrlzCells.size() >= MAX_HISTORY_NUM)
	{
		ctrlzCells.pop_front();
	}

	vector<Cell> historyCells;
	for (int i=startPoint.x; i<=endPoint.x; i++)
	{
		for (int j=startPoint.y; j<=endPoint.y; j++)
		{
			for (int k=startPoint.z; k<=endPoint.z; k++)
			{
				historyCells.push_back(Cell(glm::ivec3(i * 2, j * 2, k * 2), mesh, texture));
			}
		}
	}
	ctrlzCells.push_back(make_pair(true, historyCells));
}

void BlockManager::AddBulkCellToBlock(const vector<Cell> &cells)
{
	for (vector<Cell>::const_iterator ci = cells.begin(); ci != cells.end(); ci++)
	{
		AddCellToBlock(ci->position, ci->mesh, ci->texture);
	}
}

void BlockManager::DeleteBulkCellToBlock(const vector<Cell> &cells)
{
	for (vector<Cell>::const_iterator ci = cells.begin(); ci != cells.end(); ci++)
	{
		DeleteCellInBlock(ci->position);
	}
}

const list<Block> *BlockManager::GetBlocks()
{
	return &blocks;
}

DirectionalLight *BlockManager::GetDirectionalLight()
{
	return &dl;
}

vector<PointLight> *BlockManager::GetPointLights()
{
	return &pls;
}

vector<SpotLight> *BlockManager::GetSpotLights()
{
	return &sls;
}

void BlockManager::UpdateLightVP(DirectionalLight directionalLight)//计算平行光的VP
{
	glm::vec3 dlPos = glm::vec3(10.0f, 10.0f, -10.0f);
	glm::mat4 viewMatrix = glm::lookAt(dlPos, dlPos + directionalLight.Direction, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 orthoMatrix = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);

	lightVP = orthoMatrix * viewMatrix;
}

void BlockManager::PickCell()//鼠标拾取cell
{
	pickedCell.position = MAX_POS;
	pickRay = CalcPickRay(camera, glm::ivec4(0, 0, width, height), lastMousePos.x, lastMousePos.y);

	//射线检测
	Cell pickNearestCell;
	for (list<Block>::const_iterator bi = blocks.begin(); bi != blocks.end(); bi++)
	{
		Cell pickCell = bi->PickCell(pickRay);
		if (pickRay.ProjDisToCube(pickCell.position) < pickRay.ProjDisToCube(pickNearestCell.position))
		{
			pickNearestCell = pickCell;
		}
	}

	if (pickNearestCell.position != MAX_POS)//获取一个拾取到的cell
	{
		if (glm::distance(pickNearestCell.GetVec3Position(), camera->GetPosition()) <= maxPickedCellDis)
		{
			pickedCell = pickNearestCell;
			camera->SetPickCellPosition(pickedCell.GetVec3Position());
		}
	}
}

void BlockManager::AddCellToBlock(glm::ivec3 position, Mesh *mesh, Texture2D *texture)//根据handblock的mesh和texture确定其属于哪一个block组
{
	for (list<Block>::iterator bi = blocks.begin(); bi != blocks.end(); bi++)
	{
		if (bi->Determine(mesh, texture))
		{
			bi->AddCell(position);
			return;
		}
	}

	Block block(mesh, texture);
	block.AddCell(position);
	blocks.push_back(block);
}

void BlockManager::DeleteCellInBlock(glm::ivec3 position)
{
	for (list<Block>::iterator bi = blocks.begin(); bi != blocks.end(); bi++)
	{
		if (bi->ContainCell(position))
		{
			bi->DeleteCell(position);

			if (bi->Empty())
			{
				blocks.erase(bi);
			}
			break;
		}
	}
}

void BlockManager::DeleteCellInBlock(glm::ivec3 position, Mesh *&mesh, Texture2D *&texture)
{
	for (list<Block>::iterator bi = blocks.begin(); bi != blocks.end(); bi++)
	{
		if (bi->ContainCell(position))
		{
			mesh = bi->GetMesh();
			texture =  bi->GetTexture();

			bi->DeleteCell(position);
			
			if (bi->Empty())
			{
				blocks.erase(bi);
			}
			break;
		}
	}
}

bool BlockManager::RestLastCell()//是不是最后一个cell
{
	if (blocks.size() == 1)
	{
		if (blocks.front().cells.size() == 1)
		{
			return true;
		}
	}
	return false;
}