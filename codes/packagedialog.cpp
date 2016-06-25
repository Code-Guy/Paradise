#include "packagedialog.h"
#include "tool.h"
#include <QKeyEvent>
#include <QDesktopWidget>

using namespace std;

PackageDialog::PackageDialog(QWidget *parent) : QDialog(parent)
{
	setFixedSize(PACKAGE_DIALOG_WIDTH, PACKAGE_DIALOG_HEIGHT);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	tabWidget = new QTabWidget;
	cbScrollArea = new QScrollArea;
	dbScrollArea = new QScrollArea;
	cbWidget = new QWidget;
	dbWidget = new QWidget;
	cbGridLayout = new QGridLayout;
	dbGridLayout = new QGridLayout;
	cbCount = dbCount = 0;

	TraverseFilesInDirectory("Resources/texture/block/constructive", constructiveImgPaths, true);
	TraverseFilesInDirectory("Resources/texture/block/decorative", decorativeImgPaths, true);

	for (int i=0; i<constructiveImgPaths.size(); i++)
	{
		AddCB(constructiveImgPaths[i]);
	}
	for (int i=0; i<decorativeImgPaths.size(); i++)
	{
		AddDB(decorativeImgPaths[i]);
	}

	cbWidget->setLayout(cbGridLayout);
	dbWidget->setLayout(dbGridLayout);

	cbScrollArea->setWidget(cbWidget);
	dbScrollArea->setWidget(dbWidget);

	tabWidget->addTab(cbScrollArea, "Constructive Blocks");
	tabWidget->addTab(dbScrollArea, "Decorative Blocks");

	vLayout = new QVBoxLayout;
	vLayout->addWidget(tabWidget);

	setLayout(vLayout);

	QDesktopWidget desktop;
	int desktopHeight = desktop.geometry().height();
	int desktopWidth = desktop.geometry().width();
	move((desktopWidth - PACKAGE_DIALOG_WIDTH) / 2, (desktopHeight - PACKAGE_DIALOG_HEIGHT) / 2);
}

PackageDialog::~PackageDialog()
{
	delete tabWidget;
	delete vLayout;
}

void PackageDialog::keyPressEvent(QKeyEvent *evt)
{
	string imgPath, meshPath;
	if (activatedImgType == Construtive)
	{
		imgPath = constructiveImgPaths[activatedIndex];
		meshPath = "Resources/model/cube.obj";
	}
	else if (activatedImgType == Decorative)
	{
		imgPath = decorativeImgPaths[activatedIndex];
		meshPath = "Resources/model/cross_billboard.obj";
	}

	switch (evt->key())
	{
	case Qt::Key_Escape:
		hide();
		break;
	case Qt::Key_1:
		emit SetShortcut(0, imgPath, meshPath);
		break;
	case Qt::Key_2:
		emit SetShortcut(1, imgPath, meshPath);
		break;
	case Qt::Key_3:
		emit SetShortcut(2, imgPath, meshPath);
		break;
	case Qt::Key_4:
		emit SetShortcut(3, imgPath, meshPath);
		break;
	case Qt::Key_5:
		emit SetShortcut(4, imgPath, meshPath);
		break;
	case Qt::Key_6:
		emit SetShortcut(5, imgPath, meshPath);
		break;
	case Qt::Key_7:
		emit SetShortcut(6, imgPath, meshPath);
		break;
	case Qt::Key_8:
		emit SetShortcut(7, imgPath, meshPath);
		break;
	case Qt::Key_9:
		emit SetShortcut(8, imgPath, meshPath);
		break;
	}
}

void PackageDialog::Activate(ImgType imgType, int index)
{
	activatedImgType = imgType;
	activatedIndex = index;
}

void PackageDialog::AddCB(std::string imgPath)
{
	LabelIamge *img = new LabelIamge(imgPath, Construtive, cbCount);
	connect(img, SIGNAL(Activate(ImgType, int)), this, SLOT(Activate(ImgType, int)));
	cbGridLayout->addWidget(img, cbCount / ROW, cbCount % ROW);
	cbCount++;
}

void PackageDialog::AddDB(std::string imgPath)
{
	LabelIamge *img = new LabelIamge(imgPath, Decorative, dbCount);
	connect(img, SIGNAL(Activate(ImgType, int)), this, SLOT(Activate(ImgType, int)));
	dbGridLayout->addWidget(img, dbCount / ROW, dbCount % ROW);
	dbCount++;
}