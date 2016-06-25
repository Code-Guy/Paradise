#include "mainwindow.h"
#include "global.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowTitle(tr("Paradise"));
	resize(WIDTH, HEIGHT);

	asset = Asset::GetInstance();
	mediaPlayer = MediaPlayer::GetInstance();

	openglWidget = new OpenGLWidget;
	connect(openglWidget, SIGNAL(HideAndShowMenu()), this, SLOT(HideAndShowMenu()));
	setCentralWidget(openglWidget);

	setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
	delete asset;
	delete mediaPlayer;
	delete openglWidget;
}

void MainWindow::OpenWorld(QString worldID)
{
	openglWidget->OpenWorld(worldID);
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
	mediaPlayer->PauseMusic();
	openglWidget->SaveWorld();

	emit ShowMenu();

	if (isMaximized())
	{
		showNormal();
	}
	if (isFullScreen())
	{
		setWindowState(windowState() ^ Qt::WindowFullScreen);
	}
}

void MainWindow::HideAndShowMenu()
{
	close();
	emit ShowMenu();
}