#include "renamedialog.h"
#include "ui_renamedialog.h"
#include <QMessageBox>
#include <QPainter>

RenameDialog::RenameDialog(int initWidth, int initHeight, int fps, QWidget *parent) :
	initWidth(initWidth), initHeight(initHeight), deltaTime(1.0f / fps),
	QDialog(parent),
    ui(new Ui::RenameDialog)
{
    ui->setupUi(this);

	setWindowTitle("Rename");
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

	//°ó¶¨°´Å¥²Ûº¯Êý
	connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(OKBtnClicked()));
	connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(CancelBtnClicked()));
}

RenameDialog::~RenameDialog()
{
    delete ui;
	delete uiEffect;
}

void RenameDialog::paintEvent(QPaintEvent *evt)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	uiEffect->Logic(deltaTime);
	uiEffect->Render(&painter);
}

void RenameDialog::resizeEvent(QResizeEvent *evt)
{
	QPointF ratio = QPointF(1.0f * width() / initWidth, 1.0f * height() / initHeight);
	uiEffect->Resize(ratio);
}

void RenameDialog::SetWorldID(QStringList worldIDs, QString oldWorldID)
{
	this->worldIDs = worldIDs;
	ui->oldWorldNameLabel->setText(oldWorldID);
}

void RenameDialog::OKBtnClicked()
{
	QString newWorldID = ui->renameLineEdit->text();
	if (newWorldID == "")
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("You should write something, alright?"),
			QMessageBox::Ok);
	}
	else if(worldIDs.contains(newWorldID))
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("You've got a same world already!"),
			QMessageBox::Ok);
	}
	else
	{
		emit Rename(newWorldID);
	}
}

void RenameDialog::CancelBtnClicked()
{
	hide();
}