#include "batchcreatedialog.h"
#include "ui_batchcreatedialog.h"
#include <QKeyEvent>

BatchCreateDialog::BatchCreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatchCreateDialog)
{
    ui->setupUi(this);

	setFixedSize(BATCH_CREATE_DIALOG_WIDTH, BATCH_CREATE_DIALOG_HEIGHT);
	setWindowFlags(Qt::FramelessWindowHint);

	connect(ui->varifyPushButton, SIGNAL(clicked()), this, SLOT(VarifyBtnClicked()));
	connect(ui->addPushButton, SIGNAL(clicked()), this, SLOT(AddBtnClicked()));
	connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(CancelBtnClicked()));
}

BatchCreateDialog::~BatchCreateDialog()
{
    delete ui;
}

void BatchCreateDialog::Init(QString curTexturePath)
{
	ui->addPushButton->setEnabled(false);
	ui->validationLabel->setFont(QFont("Consolas", 14));
	ui->validationLabel->setText("<font color='red'></font>");

	ui->textureLabel->setPixmap(QPixmap(curTexturePath).scaled(QSize(LABEL_IMAGE_SIZE, LABEL_IMAGE_SIZE)));
}

void BatchCreateDialog::SetOverlapTest(bool ret)
{
	if (ret)
	{
		ui->validationLabel->setText("<font color='red'>Invalid!</font>");
		ui->addPushButton->setEnabled(false);
	}
	else
	{
		ui->validationLabel->setText("<font color='green'>Valid:)</font>");
		ui->addPushButton->setEnabled(true);
	}
}

void BatchCreateDialog::keyPressEvent(QKeyEvent *evt)
{
	switch (evt->key())
	{
	case Qt::Key_Escape:
		hide();
		break;
	}
}

void BatchCreateDialog::VarifyBtnClicked()
{
	startPoint = glm::ivec3(ui->startPointXSpinBox->text().toInt(), ui->startPointYSpinBox->text().toInt(), ui->startPointZSpinBox->text().toInt());
	endPoint = glm::ivec3(ui->endPointXSpinBox->text().toInt(), ui->endPointYSpinBox->text().toInt(), ui->endPointZSpinBox->text().toInt());
	
	emit OverlapTest(startPoint, endPoint);
}

void BatchCreateDialog::AddBtnClicked()
{
	emit AddBatchCellToBlock(startPoint, endPoint);
	hide();
}

void BatchCreateDialog::CancelBtnClicked()
{
	hide();
}