#include "setdialog.h"
#include "ui_setdialog.h"
#include "global.h"
#include <QMessageBox>
#include <QPainter>

SetDialog::SetDialog(int initWidth, int initHeight, int fps, QWidget *parent) :
	initWidth(initWidth), initHeight(initHeight), deltaTime(1.0f / fps),
	QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);

	setWindowTitle("Set");
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

	//Resolution
	if (appSet.resolution == glm::ivec2(1000, 600))
	{
		ui->resolutionComboBox->setCurrentIndex(0);
	}
	else if(appSet.resolution == glm::ivec2(640, 480))
	{
		ui->resolutionComboBox->setCurrentIndex(1);
	}
	else if(appSet.resolution == glm::ivec2(800, 600))
	{
		ui->resolutionComboBox->setCurrentIndex(2);
	}
	else if(appSet.resolution == glm::ivec2(1024, 768))
	{
		ui->resolutionComboBox->setCurrentIndex(3);
	}
	else if(appSet.resolution == glm::ivec2(1280, 720))
	{
		ui->resolutionComboBox->setCurrentIndex(4);
	}
	else if(appSet.resolution == glm::ivec2(1366, 768))
	{
		ui->resolutionComboBox->setCurrentIndex(5);
	}
	else if(appSet.resolution == glm::ivec2(1440, 900))
	{
		ui->resolutionComboBox->setCurrentIndex(6);
	}
	else if(appSet.resolution == glm::ivec2(1920, 1080))
	{
		ui->resolutionComboBox->setCurrentIndex(7);
	}

	//Display Mode
	ui->displayModeComboBox->setCurrentIndex(appSet.isFullScreen);
	//V-Sync
	ui->vsyncComboBox->setCurrentIndex(!appSet.isVSync);
	//MSAA
	if(appSet.msaa == -1)
	{
		ui->aaComboBox->setCurrentIndex(0);
	}
	else if(appSet.msaa == 2)
	{
		ui->aaComboBox->setCurrentIndex(1);
	}	
	else if(appSet.msaa == 4)
	{
		ui->aaComboBox->setCurrentIndex(2);
	}	
	else if(appSet.msaa == 8)
	{
		ui->aaComboBox->setCurrentIndex(3);
	}	
	else if(appSet.msaa == 16)
	{
		ui->aaComboBox->setCurrentIndex(4);
	}	
	//music volume
	ui->musicHorizontalSlider->setValue(appSet.musicVolume);

	//audio volume
	ui->audioHorizontalSlider->setValue(appSet.audioVolume);

	//°ó¶¨°´Å¥²Ûº¯Êý
	connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(OKBtnClicked()));
	connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(CancelBtnClicked()));
	connect(ui->aaComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(MSAAComboBoxCurrentIndexChanged()));
	connect(ui->displayModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(DisplayModeComboBoxCurrentIndexChanged(int)));
}

SetDialog::~SetDialog()
{
    delete ui;
	delete uiEffect;
}

void SetDialog::paintEvent(QPaintEvent *evt)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	uiEffect->Logic(deltaTime);
	uiEffect->Render(&painter);
}

void SetDialog::resizeEvent(QResizeEvent *evt)
{
	QPointF ratio = QPointF(1.0f * width() / initWidth, 1.0f * height() / initHeight);
	uiEffect->Resize(ratio);
}

void SetDialog::OKBtnClicked()
{
	//Resolution
	int currentResolutionIndex = ui->resolutionComboBox->currentIndex();
	switch(currentResolutionIndex)
	{
	case 0:
		appSet.resolution = glm::ivec2(1000, 600);
		break;
	case 1:
		appSet.resolution = glm::ivec2(640, 480);
		break;
	case 2:
		appSet.resolution = glm::ivec2(800, 600);
		break;
	case 3:
		appSet.resolution = glm::ivec2(1024, 768);
		break;
	case 4:
		appSet.resolution = glm::ivec2(1280, 720);
		break;
	case 5:
		appSet.resolution = glm::ivec2(1366, 768);
		break;
	case 6:
		appSet.resolution = glm::ivec2(1440, 900);
		break;
	case 7:
		appSet.resolution = glm::ivec2(1920, 1080);
		break;
	}
	//Display Mode
	appSet.isFullScreen = ui->displayModeComboBox->currentIndex();
	//V-Sync
	appSet.isVSync = !(ui->vsyncComboBox->currentIndex());
	//MSAA
	int currentMSAAIndex = ui->aaComboBox->currentIndex();
	switch(currentMSAAIndex)
	{
	case 0:
		appSet.msaa = -1;
		break;
	case 1:
		appSet.msaa = 2;
		break;
	case 2:
		appSet.msaa = 4;
		break;
	case 3:
		appSet.msaa = 8;
		break;
	case 4:
		appSet.msaa = 16;
		break;
	}
	//music volume
	appSet.musicVolume = ui->musicHorizontalSlider->value();
	//audio volume
	appSet.audioVolume = ui->audioHorizontalSlider->value();
	this->hide();
}

void SetDialog::CancelBtnClicked()
{
	this->hide();
}

void SetDialog::MSAAComboBoxCurrentIndexChanged()
{
	QMessageBox::information(NULL, tr("Information"),
		tr("The change will take effect after restart:)"),
		QMessageBox::Ok);
}

void SetDialog::DisplayModeComboBoxCurrentIndexChanged(int index)
{
	if (index == 0)
	{
		ui->resolutionComboBox->setEnabled(true);
	}
	else if(index == 1)
	{
		ui->resolutionComboBox->setEnabled(false);
	}
}