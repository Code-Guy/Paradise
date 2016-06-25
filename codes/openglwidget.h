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

	QPaintEngine* paintEngine() const { return NULL; } //避免界面闪烁

	void InitWidgetProp();//初始化widget的一些属性
	void InitOpenGLContext();//初始化window界面context
	void InitGlew();//初始化Glew
	void OpenWorld(QString worldID);
	void SaveWorld();

signals:
	void HideAndShowMenu();

protected:
	void Logic(float deltaTime);//逻辑
	void Render();//绘制

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
	QTimer drawTimer;//帧刷新计时器
	QTime *fpsTime;//FPS计时器
	float deltaTime;//上一帧绘制所需要的之间
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

	MediaPlayer *mediaPlayer;//媒体播放器

	void InitScene();//初始化场景
	void InitUIElements();//初始化UI部件

	void RenderUIElements();//绘制UI部件

	bool SetMultiSample();//设置MultiSample

	void CalcYawPitch();
};

#endif //_OPENGLWIDGET_H