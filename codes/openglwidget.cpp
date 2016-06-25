#include "openglwidget.h"
#include "overlay2d.h"
#include "primitiverenderer.h"
#include "advanceoglset.h"

#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <time.h>
#include <QMessageBox>
#include <GL/glew.h>

HGLRC _dummy_glctx;
HGLRC _real_glctx;

using namespace std;

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QWidget(parent)
{
	//初始化widget的一些属性
	InitWidgetProp();
	//初始化window界面context
	InitOpenGLContext();
	//设置MultiSample
	SetMultiSample();
	//初始化Glew
	InitGlew();
	
	if (!InitVSync())
	{
		printf("Init V-Sync failed!\n");
	}

	asset = Asset::GetInstance();//初始化资源库
	camera = asset->GetCamera();//获取统一的camera
	mediaPlayer = MediaPlayer::GetInstance();

	//初始化场景
	InitScene();
	InitUIElements();

	//设置绘制计时器
	connect(&drawTimer, SIGNAL(timeout()), this, SLOT(update()));
	drawTimer.start(0);//以最高帧率绘制

	mouseSensitivity = 0.15f;//鼠标灵敏度
	yaw = pitch = 0.0f;
	deltaMouseX = deltaMouseY = 0.0f;
	isInit = true;
	
	connect(batchCreateDialog, SIGNAL(OverlapTest(glm::ivec3, glm::ivec3)), this, SLOT(OverlapTest(glm::ivec3, glm::ivec3)));
	connect(batchCreateDialog, SIGNAL(AddBatchCellToBlock(glm::ivec3, glm::ivec3)), this, SLOT(AddBatchCellToBlock(glm::ivec3, glm::ivec3)));

	srand((unsigned int)time(NULL));//初始化随机数种子
}

OpenGLWidget::~OpenGLWidget()
{
	delete fpsTime;
	delete blockMsg;
	delete packageDialog;

	Overlay2D::Release();
	PrimitiveRenderer::Release();

	wglDeleteContext(_dummy_glctx);
	wglDeleteContext(_real_glctx);
}

void OpenGLWidget::InitWidgetProp()//初始化widget的一些属性
{
	setWindowOpacity(1);
	setObjectName(QString::fromUtf8("OpenGLWidget"));
	//setAcceptDrops(true);
	setMouseTracking(true);
	setContextMenuPolicy(Qt::PreventContextMenu);
	setFocusPolicy(Qt::WheelFocus);
	//setWindowFlags(Qt::FramelessWindowHint);
	//setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_OpaquePaintEvent); // WA_NoBackground is obsolete
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_InputMethodEnabled);  // To enable IME event capture
	//设置鼠标光标图片
	QCursor myCursor(QPixmap("Resources/texture/sword_cursor.png"));
	setCursor(myCursor);
}

void OpenGLWidget::InitOpenGLContext()//初始化window界面context
{
	QMessageBox *box = new QMessageBox;
	HWND hwnd = (HWND)box->winId();
	HDC hdc = GetDC(hwnd);
	//[METHOD] SetWindowPixelFormat(HDC)

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                        // nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER
		| PFD_SUPPORT_GDI/*|PFD_STEREO*/,    // dwFlags
		PFD_TYPE_RGBA,            // PixelType
		24,                       // ColorBits
		0, 0, 0, 0, 0, 0,         // RGB bits and shifts
		8,                        // cAlphaBits
		0,                        // cAlphaShift
		0, 0, 0, 0, 0,            // Accum
		24,                       // Depth
		8,                        // Stencil
		0,                        // cAuxBuffers
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pixelFormat;
	pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (pixelFormat == 0) {
		pixelFormat = 1;
		if (DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd) == 0)
			return;
	}

	if (SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE) return;

	//[NOTE] !DO NOT! change check order
	if (!(_dummy_glctx = wglCreateContext(hdc)) || !wglMakeCurrent(hdc, _dummy_glctx))
		return;
}

void OpenGLWidget::InitGlew()//初始化Glew
{
	//初始化glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW\n");
		exit(-1);
	}
}

void OpenGLWidget::OpenWorld(QString worldID)
{
	camera->Reset();
	SetVSync(appSet.isVSync);

	mediaPlayer->SetMusicVolume(appSet.musicVolume);
	mediaPlayer->SetAudioVolume(appSet.audioVolume);
	mediaPlayer->PlayMusic();

	vector<QString> shortcutMeshPaths;
	vector<QString> shortcutTexPaths;
	blockMsg->OpenWorld(worldID, shortcutMeshPaths, shortcutTexPaths);
	shortcutBar->SetMeshPaths(shortcutMeshPaths);
	shortcutBar->SetTexturePaths(shortcutTexPaths);

	outlineWidget->Init(blockMsg->GetBlocks(), blockMsg->GetDirectionalLight(), blockMsg->GetPointLights(), blockMsg->GetSpotLights());
}

void OpenGLWidget::paintEvent(QPaintEvent* evt)
{
	deltaTime = fpsTime->elapsed();
	deltaTime /= 1000.0f;
	fpsTime->restart();
	fpsTimer += deltaTime;
	fpsCounter++;

	Logic(deltaTime);

	Render();

	HWND hwnd = (HWND) winId();
	HDC hdc = GetDC(hwnd);
	SwapBuffers(hdc);
	ReleaseDC(hwnd, hdc);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *evt)
{
	if (hasFocus())
	{
		handBlock->MousePressEvent(evt);
		blockMsg->MousePressEvent(evt, handBlock->GetMesh(), handBlock->GetTexture());
	}
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *evt)
{
	blockMsg->MouseMoveEvent(evt);
}

void OpenGLWidget::resizeEvent(QResizeEvent *evt)
{
	glViewport(0, 0, width(), height());
	blockMsg->Resize(width(), height());
	Overlay2D::Resize(width(), height());
	camera->SetAspectRatio(width(), height());
}

void OpenGLWidget::keyPressEvent(QKeyEvent *evt)
{
	camera->KeyPressEvent(evt);
	shortcutBar->KeyPressEvent(evt);
	mediaPlayer->KeyPressEvent(evt);
	blockMsg->KeyPressEvent(evt);

	switch (evt->key())
	{
	case Qt::Key_E:
		if (packageDialog->isHidden())
		{
			packageDialog->exec();
		}
		break;
	case Qt::Key_B:
		if (batchCreateDialog->isHidden())
		{
			batchCreateDialog->Init(shortcutBar->GetCurTexturePath());
			batchCreateDialog->exec();
		}
		break;
	case Qt::Key_O:
		if (outlineWidget->isHidden())
		{
			outlineWidget->show();
		}
		else
		{
			outlineWidget->activateWindow();
		}
		
		break;
	case Qt::Key_Escape:
		outlineWidget->hide();
		int ret = QMessageBox::warning(NULL, tr("Warning"),
			tr("Are you sure to return to the main menu?"),
			QMessageBox::Yes | QMessageBox::No);
		if (ret == QMessageBox::Yes)
		{
			emit HideAndShowMenu();
		}
		break;
	}
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *evt)
{
	camera->KeyReleaseEvent(evt);
}

void OpenGLWidget::wheelEvent(QWheelEvent *evt)
{
	int numSteps = evt->delta() / 120;//滚动的步数
	shortcutBar->UpdateSelectedSlotIndex(numSteps);
}

void OpenGLWidget::SetShortcut(int slotIndex, string imgPath, string meshPath)
{
	shortcutBar->SetShortcutItem(slotIndex, imgPath, meshPath);
}

void OpenGLWidget::OverlapTest(glm::ivec3 startPoint, glm::ivec3 endPoint)
{
	bool ret = blockMsg->OverlapTest(startPoint, endPoint);
	batchCreateDialog->SetOverlapTest(ret);
}

void OpenGLWidget::AddBatchCellToBlock(glm::ivec3 startPoint, glm::ivec3 endPoint)
{
	blockMsg->AddBatchCellToBlock(startPoint, endPoint, handBlock->GetMesh(), handBlock->GetTexture());
}

void OpenGLWidget::SaveWorld()
{
	blockMsg->SaveWorld(shortcutBar->GetMeshPaths(), shortcutBar->GetTexturePaths());
	outlineWidget->hide();
}

void OpenGLWidget::InitScene()//初始化场景
{
	glClearColor(0, 1, 0, 1);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//初始化天空盒
	skybox = new Skybox();
	if (!skybox->Init("Resources/texture/skybox/fantasy/", "tga"))
	{
		printf("Error initializing the skybox\n");
	}

	handBlock = new HandBlock();

	//初始化blockMsg
	blockMsg = new BlockManager();
}

void OpenGLWidget::InitUIElements()//初始化UI部件
{
	//初始化图元绘制器
	PrimitiveRenderer::Init();

	//初始化图标元素
	shortcutBar = new ShortcutBar;
	//初始化2d文字
	Overlay2D::Init("Resources/texture/Batang_Bold.tga");

	fpsTimer = 0;
	fpsCounter = 0;

	//初始化FPS计时器
	fpsTime = new QTime;
	fpsTime->start();

	packageDialog = new PackageDialog;
	packageDialog->hide();

	batchCreateDialog = new BatchCreateDialog;
	batchCreateDialog->hide();

	outlineWidget = new OutlineWidget;
	outlineWidget->hide();

	connect(packageDialog, SIGNAL(SetShortcut(int, std::string, std::string)), this, SLOT(SetShortcut(int, std::string, std::string)));
}

void OpenGLWidget::RenderUIElements()//绘制UI部件
{
	shortcutBar->Render();
	Overlay2D::Print(string("FPS:") + fpsStr.toStdString(), 830, 550, 20, glm::vec4(1, 1, 0, 1));
	Overlay2D::Print(pickedCellCoordStr.toStdString(), 480, 580, 10, glm::vec4(0, 1, 1, 1), 0.05);
}

bool OpenGLWidget::SetMultiSample()//设置MultiSample
{
	HWND hwnd = (HWND) winId();
	HDC hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                        // nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER
		| PFD_SUPPORT_GDI/*|PFD_STEREO*/,    // dwFlags
		PFD_TYPE_RGBA,            // PixelType
		24,                       // ColorBits
		0, 0, 0, 0, 0, 0,         // RGB bits and shifts
		8,                        // cAlphaBits
		0,                        // cAlphaShift
		0, 0, 0, 0, 0,            // Accum
		24,                       // Depth
		8,                        // Stencil
		0,                        // cAuxBuffers
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	int pixelFormat;
	if (appSet.msaa == -1)
	{
		pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if (pixelFormat == 0) {
			pixelFormat = 1;
			if (DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd) == 0)
				return false;
		}
	}
	else if (!InitMultisample(hwnd, appSet.msaa, pixelFormat))
	{
		return false;
	}
	if (SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE) 
		return false;

	//[NOTE] !DO NOT! change check order
	if (!(_real_glctx = wglCreateContext(hdc)) || !wglMakeCurrent(hdc, _real_glctx))
		return false;

	return true;
}

void OpenGLWidget::CalcYawPitch()
{
	QPoint localMousePos = this->mapFromGlobal(cursor().pos());
	if (isInit)
	{
		lastMouseX = localMousePos.x();
		lastMouseY = localMousePos.y();
		isInit = false;
	}

	if (localMousePos.x() <= 0 || localMousePos.x() >= width() - 1)
	{
		QCursor c = cursor();
		c.setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
		setCursor(c);
		lastMouseX = width() / 2;
		lastMouseY = height() / 2;
	}

	deltaMouseX = localMousePos.x() - lastMouseX;
	deltaMouseY = localMousePos.y() - lastMouseY;

	yaw += deltaMouseX * mouseSensitivity;
	pitch -= deltaMouseY * mouseSensitivity;

	lastMouseX = localMousePos.x();
	lastMouseY = localMousePos.y();
}

void OpenGLWidget::Logic(float deltaTime)//逻辑
{
	if (hasFocus())
	{
		CalcYawPitch();
	}
	else
	{
		isInit = true;
	}

	camera->Update(yaw, pitch, deltaTime);

	blockMsg->Logic(deltaTime);
	handBlock->Logic(deltaTime);
	handBlock->SetTexture(shortcutBar->GetSelectedSlotTexture());
	handBlock->SetMesh(shortcutBar->GetSelectedSlotMesh());
	outlineWidget->Update();

	if (fpsTimer >= 0.5f)
	{
		fpsStr = QString("%1").arg((int)(fpsCounter / fpsTimer));
		fpsTimer = 0;
		fpsCounter = 0;
	}

	glm::ivec3 pickedCellPos = blockMsg->GetPickedCellPos();
	if (pickedCellPos != MAX_POS)
	{
		pickedCellCoordStr = QString("%1, %2, %3").arg(pickedCellPos.x / 2).arg(pickedCellPos.y / 2).arg(pickedCellPos.z / 2);
	}
}

void OpenGLWidget::Render()//绘制
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//用背景色清除颜色缓冲
	if (appSet.msaa != -1)
	{
		glEnable(GL_MULTISAMPLE_ARB);
	}
	skybox->Render();
 	blockMsg->Render();
 	handBlock->Render();
 
	RenderUIElements();

	if (appSet.msaa != -1)
	{
		glDisable(GL_MULTISAMPLE_ARB);
	}
}