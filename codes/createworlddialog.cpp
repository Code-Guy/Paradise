#include "createworlddialog.h"
#include "ui_createworlddialog.h"
#include <QMessageBox>
#include <QPainter>

CreateWorldDialog::CreateWorldDialog(int initWidth, int initHeight, int fps, QWidget *parent) :
    initWidth(initWidth), initHeight(initHeight), deltaTime(1.0f / fps),
	QDialog(parent),
    ui(new Ui::CreateWorldDialog)
{
    ui->setupUi(this);

	setWindowTitle("Create World");
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

	connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(OKBtnClicked()));
	connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(CancelBtnClicked()));
}

CreateWorldDialog::~CreateWorldDialog()
{
    delete ui;
	delete uiEffect;
}

void CreateWorldDialog::paintEvent(QPaintEvent *evt)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	uiEffect->Logic(deltaTime);
	uiEffect->Render(&painter);
}

void CreateWorldDialog::resizeEvent(QResizeEvent *evt)
{
	QPointF ratio = QPointF(1.0f * width() / initWidth, 1.0f * height() / initHeight);
	uiEffect->Resize(ratio);
}

void CreateWorldDialog::OKBtnClicked()
{
	if(ui->worldNameLineEdit->text() == "")
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("The world name can't be empty!"),
			QMessageBox::Ok);
	}
	else
	{
		emit CreateWorld(ui->worldNameLineEdit->text().trimmed());
	}
}

void CreateWorldDialog::CancelBtnClicked()
{
	hide();
}

void CreateWorldDialog::CreateWorldVerify(bool validation)
{
	if (!validation)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("You have already created this world!"),
			QMessageBox::Ok);
	}
	else
	{
		hide();
	}
}