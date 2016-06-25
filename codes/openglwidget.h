#ifndef _OPENGLWIDGET_H
#define _OPENGLWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>

#include "blockmanager.h"
#include "skybox.h"
#include "packagedialog.h"
#include "batchcreatedialog.h"
#include "shortcutbar.h"
#include "handblock.h"
#include "mediaplayer.h"
#include "outlinewidget.h"

class OpenGLWidget : public QWidget
{
	Q_OBJECT;

public:
	OpenGLWidget(QWidget* parent = 0);
	~OpenGLWidget();

	QPaintEngine* paintEngine() const { return NULL; } //���������˸

	void InitWidgetProp();//��ʼ��widget��һЩ����
	void InitOpenGLContext();//��ʼ��window����context
	void InitGlew();//��ʼ��Glew
	void OpenWorld(QString worldID);
	void SaveWorld();

signals:
	void HideAndShowMenu();

protected:
	void Logic(float deltaTime);//�߼�
	void Render();//����

	virtual void paintEvent(QPaintEvent* evt);
	virtual void mousePressEvent(QMouseEvent *evt);
	virtual void mouseMoveEvent(QMouseEvent *evt);
	virtual void resizeEvent(QResizeEvent *evt);

	virtual void keyPressEvent(QKeyEvent *evt);
	virtual void keyReleaseEvent(QKeyEvent *evt);

	virtual void wheelEvent(QWheelEvent *evt);

private slots:
	void SetShortcut(int slotIndex, std::string imgPath, std::string meshPath);
	void OverlapTest(glm::ivec3 startPoint, glm::ivec3 endPoint);
	void AddBatchCellToBlock(glm::ivec3 startPoint, glm::ivec3 endPoint);

private:
	QTimer drawTimer;//֡ˢ�¼�ʱ��
	QTime *fpsTime;//FPS��ʱ��
	float deltaTime;//��һ֡��������Ҫ��֮��
	float fpsTimer;
	int fpsCounter;
	QString fpsStr;
	QString pickedCellCoordStr;

	float yaw;
	float pitch;

	bool isInit;

	int deltaMouseX, deltaMouseY;
	int lastMouseX, lastMouseY;

	float mouseSensitivity;

	Camera *camera;
	Skybox *skybox;

	BlockManager *blockMsg;

	PackageDialog *packageDialog;
	BatchCreateDialog *batchCreateDialog;
	OutlineWidget *outlineWidget;

	ShortcutBar *shortcutBar;

	HandBlock *handBlock;

	Asset *asset;

	MediaPlayer *mediaPlayer;//ý�岥����

	void InitScene();//��ʼ������
	void InitUIElements();//��ʼ��UI����

	void RenderUIElements();//����UI����

	bool SetMultiSample();//����MultiSample

	void CalcYawPitch();
};

#endif //_OPENGLWIDGET_H