#ifndef _BLOCKMANAGER_H
#define  _BLOCKMANAGER_H

#include "block.h"
#include "lightshader.h"
#include "shadowmapshader.h"
#include "shadowmapfbo.h"
#include "ray.h"
#include "camera.h"
#include "particlesystem.h"
#include "mediaplayer.h"
#include "billboard.h"

#include <QString>
#include <QMouseEvent>
#include <QKeyEvent>
#include <deque>

#define DEFAULT_MAX_PICKED_CELL_DIS 100 //鼠标拾取到的cell到摄像机的默认最大距离
#define MAX_HISTORY_NUM 50 //最多存储50条历史记录

class BlockManager
{
public:
	BlockManager();
	~BlockManager();

	void Logic(float deltaTime);
	void Render();

	void Resize(int width, int height);

	void MousePressEvent(QMouseEvent *evt, Mesh *mesh, Texture2D *texture);
	void MouseMoveEvent(QMouseEvent *evt);

	void KeyPressEvent(QKeyEvent *evt);

	glm::ivec3 GetPickedCellPos();
	void SetMaxPickedCellDis(float maxPickedCellDis);

	void OpenWorld(QString worldID, std::vector<QString> &shortcutMeshPaths, std::vector<QString> &shortcutTexPaths);
	void SaveWorld(const std::vector<QString> &shortcutMeshPaths, const std::vector<QString> &shortcutTexPaths);

	bool OverlapTest(glm::ivec3 startPoint, glm::ivec3 endPoint);

	void AddBatchCellToBlock(glm::ivec3 startPoint, glm::ivec3 endPoint, Mesh *mesh, Texture2D *texture);
	void AddBulkCellToBlock(const std::vector<Cell> &cells);
	void DeleteBulkCellToBlock(const std::vector<Cell> &cells);

	const std::list<Block> *GetBlocks();
	DirectionalLight *GetDirectionalLight();
	std::vector<PointLight> *GetPointLights();
	std::vector<SpotLight> *GetSpotLights();

private:
	QString worldID;
	Camera *camera;
	std::list<Block> blocks;//mesh/texture对共同确定一组block

	LightShader *lightShader;
	ShadowMapShader *shadowMapShader;
	ShadowMapFBO *shaderMapFBO;

	PSEffect *psEffect;//粒子管理器
	MediaPlayer *mediaPlayer;//媒体播放器

	glm::mat4 lightVP;

	/*三种灯光*/
	DirectionalLight dl;
	std::vector<PointLight> pls;
	std::vector<SpotLight> sls;

	Billboard dlBd;
	std::vector<Billboard> plBds;
	std::vector<Billboard> slBds;

	int width, height;//viewport宽高

	glm::ivec2 lastMousePos;//上一次鼠标位置
	Ray pickRay;//鼠标拾取射线
	Cell pickedCell;//鼠标拾取到的cell
	float maxPickedCellDis;//鼠标拾取到的cell到摄像机的最大距离

	std::deque< std::pair<bool, std::vector<Cell>> > ctrlzCells;
	std::deque< std::pair<bool, std::vector<Cell>> > ctrlyCells;

	void UpdateLightVP(DirectionalLight directionalLight);//计算平行光的VP
	void PickCell();//鼠标拾取cell
	void AddCellToBlock(glm::ivec3 position, Mesh *mesh, Texture2D *texture);//根据handblock的mesh和texture确定其属于哪一个block组
	void DeleteCellInBlock(glm::ivec3 position);
	void DeleteCellInBlock(glm::ivec3 position, Mesh *&mesh, Texture2D *&texture);

	bool RestLastCell();//是不是最后一个cell
};

#endif //_BLOCKMANAGER_H