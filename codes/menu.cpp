#include "menu.h"
#include "ui_menu.h"
#include "tool.h"
#include "jsonhandler.h"
#include <vector>
#include <string>
#include <QPainter>
#include <QDesktopWidget>

using namespace std;

Menu::Menu(int initWidth, int initHeight, int fps, QWidget *parent) :
	initWidth(initWidth), initHeight(initHeight), deltaTime(1.0f / fps),
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

	setWindowTitle("Paradise");
	resize(initWidth, initHeight);
	uiEffect = new UIEffect(3, 12,
		5, 80,
		20, 40,
		QPoint(initWidth, initHeight),
		-360, 360,
		40, 80,
		QColor(50, 50, 50), QColor(230, 230, 230),
		QColor(50, 50, 50), QColor(230, 230, 230),
		1, 8,
		1, 5,
		3,
		3);
	connect(&timer, SIGNAL(timeout()), this, SLOT(repaint()));
	timer.start(1000.0f / fps);

	JsonHandler::ReadAppSet();

	//获取所有世界名称
	vector<string> worldIDStrs;
	TraverseFilesInDirectory("Resources/world", worldIDStrs, false);
	for (int i=0; i<worldIDStrs.size(); i++)
	{
		QString worldIDStr = QString::fromStdString(worldIDStrs[i]);
		worldIDs.push_back(worldIDStr.section(".", 0, 0));
	}

	selectWorldDialog = new SelectWorldDialog(394, 271);
	setDialog = new SetDialog(360, 334);
	helpDialog = new HelpDialog(563, 607);

	mainWindow = new MainWindow;

	//绑定按钮槽函数
	connect(ui->selectedWorldBtn, SIGNAL(clicked()), this, SLOT(SelectWorldBtnClicked()));
	connect(ui->setBtn, SIGNAL(clicked()), this, SLOT(SetBtnClicked()));
	connect(ui->helpBtn, SIGNAL(clicked()), this, SLOT(HelpBtnClicked()));
	connect(ui->exitBtn, SIGNAL(clicked()), this, SLOT(ExitBtnClicked()));
	//绑定createWorldDialog槽函数
	connect(selectWorldDialog, SIGNAL(CreateWorld(QString)), this, SLOT(CreateWorld(QString)));
	connect(this, SIGNAL(CreateWorldVerify(bool)), selectWorldDialog, SIGNAL(CreateWorldVerify(bool)));
	//绑定selectWorldDialog槽函数 
	connect(selectWorldDialog, SIGNAL(DeleteWorld(QString)), this, SLOT(DeleteWorld(QString)));
	connect(selectWorldDialog, SIGNAL(OpenWorld(QString)), this, SLOT(OpenWorld(QString)));
	connect(selectWorldDialog, SIGNAL(RenameWorld(QString, QString)), this, SLOT(RenameWorld(QString, QString)));
	//绑定mainWindow槽函数 
	connect(mainWindow, SIGNAL(ShowMenu()), this, SLOT(ShowMenu()));
}

Menu::~Menu()
{
    delete ui;

	delete uiEffect;
	delete selectWorldDialog;
	delete setDialog;
	delete helpDialog;

	delete mainWindow;
}

void Menu::paintEvent(QPaintEvent *evt)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	uiEffect->Logic(deltaTime);
	uiEffect->Render(&painter);
}

void Menu::closeEvent(QCloseEvent *evt)
{
	JsonHandler::WriteAppSet();
}

void Menu::resizeEvent(QResizeEvent *evt)
{
	QPointF ratio = QPointF(1.0f * width() / initWidth, 1.0f * height() / initHeight);
	uiEffect->Resize(ratio);
}

void Menu::SelectWorldBtnClicked()
{
	selectWorldDialog->InitListWidget(worldIDs);
	selectWorldDialog->exec();
}

void Menu::SetBtnClicked()
{
	setDialog->exec();
}

void Menu::HelpBtnClicked()
{
	helpDialog->exec();
}

void Menu::ExitBtnClicked()
{
	close();
}

void Menu::CreateWorld(QString worldID)
{
	if (worldIDs.contains(worldID))
	{
		emit CreateWorldVerify(false);
	}
	else
	{
		emit CreateWorldVerify(true);
		worldIDs.push_back(worldID);
		JsonHandler::WriteBlocksDefault(worldID);
		selectWorldDialog->InitListWidget(worldIDs);
	}
}

void Menu::DeleteWorld(QString worldID)
{
	worldIDs.removeOne(worldID);
	QFile::remove(JsonHandler::WorldJsonFilePath(worldID));
}

void Menu::OpenWorld(QString worldID)
{
	this->hide();

	mainWindow->OpenWorld(worldID);
	if (appSet.isFullScreen)
	{
		mainWindow->showFullScreen();
	}
	else
	{
		QDesktopWidget desktop;
		int desktopHeight = desktop.geometry().height();
		int desktopWidth = desktop.geometry().width(); 
		mainWindow->setGeometry((desktopWidth - appSet.resolution.x) / 2, (desktopHeight - appSet.resolution.y) / 2, 
			appSet.resolution.x, appSet.resolution.y);
		mainWindow->show();
	}
}

void Menu::RenameWorld(QString oldWorldID, QString newWorldID)
{
	for (int i=0; i<worldIDs.size(); i++)
	{
		if (worldIDs[i] == oldWorldID)
		{
			worldIDs[i] = newWorldID;
			break;
		}
	}
	QFile::rename(JsonHandler::WorldJsonFilePath(oldWorldID), JsonHandler::WorldJsonFilePath(newWorldID));
}

void Menu::ShowMenu()
{
	this->show();
}