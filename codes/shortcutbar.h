#ifndef _SHORTCUTBAR_H
#define  _SHORTCUTBAR_H

#include "overlay2d.h"
#include "asset.h"
#include "mesh.h"
#include <QKeyEvent>
#include <QString>
#include <vector>

#define BLOCK_TEXTURE_NUM 9

#define SelectedSlotColor glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)

class ShortcutBar
{
public:
	ShortcutBar();
	~ShortcutBar();

	void Render();
	void SetShortcutItem(int slotIndex, std::string imgPath, std::string meshPath);
	void UpdateSelectedSlotIndex(int addStep);

	Texture2D *GetSelectedSlotTexture();
	Mesh *GetSelectedSlotMesh();

	void KeyPressEvent(QKeyEvent *evt);

	std::vector<QString> GetTexturePaths();
	QString GetCurTexturePath();
	void SetTexturePaths(const std::vector<QString> &texturePaths);
	std::vector<QString> GetMeshPaths();
	void SetMeshPaths(const std::vector<QString> &meshPaths);

private:
	Texture2D *bgMenuTex;
	Texture2D *selectedSlotTex;

	Texture2D *blockTexs[BLOCK_TEXTURE_NUM]; 
	Mesh *blockMeshes[BLOCK_TEXTURE_NUM];

	Asset *asset;

	int selectedSlotIndex;
};

#endif //_SHORTCUTBAR_H