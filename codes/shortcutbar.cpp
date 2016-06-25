#include "shortcutbar.h"

using namespace std;

ShortcutBar::ShortcutBar()
{
	asset = Asset::GetInstance();

	bgMenuTex = asset->GetTexture("Resources/texture/bgMenu.jpg", GL_NEAREST, GL_NEAREST);
	selectedSlotTex = asset->GetTexture("Resources/texture/selected_slot.jpg", GL_NEAREST, GL_NEAREST);
 	Texture2D *defaultBlockTex = asset->GetTexture("Resources/texture/block/constructive/default_block_tex.png", GL_NEAREST, GL_NEAREST);
	Mesh *defaultBlockMesh = asset->GetMesh("Resources/model/cube.obj");

 	for (int i=0; i<BLOCK_TEXTURE_NUM; i++)
 	{
 		blockTexs[i] = defaultBlockTex;
		blockMeshes[i] = defaultBlockMesh;
 	}

	selectedSlotIndex = 0;
}

ShortcutBar::~ShortcutBar()
{

}

void ShortcutBar::Render()
{
	for (int i=0; i<BLOCK_TEXTURE_NUM; i++)
	{
		if (i == selectedSlotIndex)
		{
			Overlay2D::Draw(blockTexs[i], 280 + i * 50, 36, 40, 40, SelectedSlotColor);
			Overlay2D::Draw(selectedSlotTex, 273 + i * 50, 28);
		}
		Overlay2D::Draw(blockTexs[i], 280 + i * 50, 36, 40, 40);
	}

 	Overlay2D::Draw(bgMenuTex, 256, 30, glm::vec2(0.9f, 0.9f));
}

void ShortcutBar::SetShortcutItem(int slotIndex, string imgPath, string meshPath)
{
	blockTexs[slotIndex] = asset->GetTexture(imgPath, GL_NEAREST, GL_NEAREST);
	blockMeshes[slotIndex] = asset->GetMesh(meshPath);
}

void ShortcutBar::UpdateSelectedSlotIndex(int addStep)
{
	selectedSlotIndex += addStep;
	if (selectedSlotIndex < 0)
	{
		selectedSlotIndex = BLOCK_TEXTURE_NUM + selectedSlotIndex;
	}
	else if(selectedSlotIndex > BLOCK_TEXTURE_NUM - 1)
	{
		selectedSlotIndex = selectedSlotIndex - BLOCK_TEXTURE_NUM;
	}
}

Texture2D *ShortcutBar::GetSelectedSlotTexture()
{
	return blockTexs[selectedSlotIndex];
}

Mesh *ShortcutBar::GetSelectedSlotMesh()
{
	return blockMeshes[selectedSlotIndex];
}

void ShortcutBar::KeyPressEvent(QKeyEvent *evt)
{
	switch(evt->key())
	{
	case Qt::Key_1:
		selectedSlotIndex = 0;
		break;
	case Qt::Key_2:
		selectedSlotIndex = 1;
		break;
	case Qt::Key_3:
		selectedSlotIndex = 2;
		break;
	case Qt::Key_4:
		selectedSlotIndex = 3;
		break;
	case Qt::Key_5:
		selectedSlotIndex = 4;
		break;
	case Qt::Key_6:
		selectedSlotIndex = 5;
		break;
	case Qt::Key_7:
		selectedSlotIndex = 6;
		break;
	case Qt::Key_8:
		selectedSlotIndex = 7;
		break;
	case Qt::Key_9:
		selectedSlotIndex = 8;
		break;
	}
}

vector<QString> ShortcutBar::GetTexturePaths()
{
	vector<QString> texturePaths;
	for (int i=0; i<BLOCK_TEXTURE_NUM; i++)
	{
		texturePaths.push_back(QString::fromStdString(asset->GetTexturePath(blockTexs[i])));
	}

	return texturePaths;
}

QString ShortcutBar::GetCurTexturePath()
{
	vector<QString> texturePaths = GetTexturePaths();
	return texturePaths[selectedSlotIndex];
}

void ShortcutBar::SetTexturePaths(const std::vector<QString> &texturePaths)
{
	for (int i=0; i<BLOCK_TEXTURE_NUM; i++)
	{
		blockTexs[i] = asset->GetTexture(texturePaths[i].toStdString(), GL_NEAREST, GL_NEAREST);
	}
}

std::vector<QString> ShortcutBar::GetMeshPaths()
{
	vector<QString> meshPaths;
	for (int i=0; i<BLOCK_TEXTURE_NUM; i++)
	{
		meshPaths.push_back(QString::fromStdString(asset->GetMeshPath(blockMeshes[i])));
	}

	return meshPaths;
}

void ShortcutBar::SetMeshPaths(const std::vector<QString> &meshPaths)
{
	for (int i=0; i<BLOCK_TEXTURE_NUM; i++)
	{
		blockMeshes[i] = asset->GetMesh(meshPaths[i].toStdString());
	}
}