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

#define DEFAULT_MAX_PICKED_CELL_DIS 100 //���ʰȡ����cell���������Ĭ��������
#define MAX_HISTORY_NUM 50 //���洢50����ʷ��¼

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
	std::list<Block> blocks;//mesh/texture�Թ�ͬȷ��һ��block

	LightShader *lightShader;
	ShadowMapShader *shadowMapShader;
	ShadowMapFBO *shaderMapFBO;

	PSEffect *psEffect;//���ӹ�����
	MediaPlayer *mediaPlayer;//ý�岥����

	glm::mat4 lightVP;

	/*���ֵƹ�*/
	DirectionalLight dl;
	std::vector<PointLight> pls;
	std::vector<SpotLight> sls;

	Billboard dlBd;
	std::vector<Billboard> plBds;
	std::vector<Billboard> slBds;

	int width, height;//viewport���

	glm::ivec2 lastMousePos;//��һ�����λ��
	Ray pickRay;//���ʰȡ����
	Cell pickedCell;//���ʰȡ����cell
	float maxPickedCellDis;//���ʰȡ����cell���������������

	std::deque< std::pair<bool, std::vector<Cell>> > ctrlzCells;
	std::deque< std::pair<bool, std::vector<Cell>> > ctrlyCells;

	void UpdateLightVP(DirectionalLight directionalLight);//����ƽ�й��VP
	void PickCell();//���ʰȡcell
	void AddCellToBlock(glm::ivec3 position, Mesh *mesh, Texture2D *texture);//����handblock��mesh��textureȷ����������һ��block��
	void DeleteCellInBlock(glm::ivec3 position);
	void DeleteCellInBlock(glm::ivec3 position, Mesh *&mesh, Texture2D *&texture);

	bool RestLastCell();//�ǲ������һ��cell
};

#endif //_BLOCKMANAGER_H