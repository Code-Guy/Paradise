#include "selectworlddialog.h"
#include "ui_selectworlddialog.h"
#include <QPainter>

SelectWorldDialog::SelectWorldDialog(int initWidth, int initHeight, int fps, QWidget *parent) :
	initWidth(initWidth), initHeight(initHeight), deltaTime(1.0f / fps),
	QDialog(parent),
    ui(new Ui::SelectWorldDialog)
{
    ui->setupUi(this);

	setWindowTitle("World Manager");
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

	renameDialog = new RenameDialog(414, 157);
	createWorldDialog = new CreateWorldDialog(400, 195);

	//默认按钮都点不了
	ui->renameBtn->setEnabled(false);
	ui->deleteBtn->setEnabled(false);

	//绑定按钮槽函数
	connect(ui->createBtn, SIGNAL(clicked()), this, SLOT(CreateBtnClicked()));
	connect(ui->renameBtn, SIGNAL(clicked()), this, SLOT(RenameClicked()));
	connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(DeleteBtnClicked()));

	//绑定listwidget槽函数
	connect(ui->worldListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(ItemActivated(QListWidgetItem *)));
	connect(ui->worldListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(ItemDoubleClicked(QListWidgetItem *)));

	//绑定renameDialog槽函数
	connect(renameDialog, SIGNAL(Rename(QString)), this, SLOT(Rename(QString)));

	connect(createWorldDialog, SIGNAL(CreateWorld(QString)), this, SIGNAL(CreateWorld(QString)));
	connect(this, SIGNAL(CreateWorldVerify(bool)), createWorldDialog, SLOT(CreateWorldVerify(bool)));
}

SelectWorldDialog::~SelectWorldDialog()
{
    delete ui;
	delete uiEffect;
	delete renameDialog;
	delete createWorldDialog;
}

void SelectWorldDialog::paintEvent(QPaintEvent *evt)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	uiEffect->Logic(deltaTime);
	uiEffect->Render(&painter);
}

void SelectWorldDialog::resizeEvent(QResizeEvent *evt)
{
	QPointF ratio = QPointF(1.0f * width() / initWidth, 1.0f * height() / initHeight);
	uiEffect->Resize(ratio);
}

void SelectWorldDialog::InitListWidget(QStringList worldIDs)
{
	ui->worldListWidget->clear();
	//初始化listwidget
	for (int i=0; i<worldIDs.size(); i++)
	{
		ui->worldListWidget->addItem(worldIDs.at(i));
	}
	ui->worldListWidget->clearFocus();
	//默认按钮都点不了
	ui->renameBtn->setEnabled(false);
	ui->deleteBtn->setEnabled(false);
}

void SelectWorldDialog::CreateBtnClicked()
{
	createWorldDialog->exec();
}

void SelectWorldDialog::RenameClicked()
{
	QStringList worldIDs;
	for (int i=0; i<ui->worldListWidget->count(); i++)
	{
		worldIDs.push_back(ui->worldListWidget->item(i)->text());
	}

	renameDialog->SetWorldID(worldIDs, ui->worldListWidget->currentItem()->text());
	renameDialog->exec();

	ui->renameBtn->setEnabled(false);
	ui->deleteBtn->setEnabled(false);
}

void SelectWorldDialog::DeleteBtnClicked()
{
	QListWidgetItem *currentItem = ui->worldListWidget->currentItem();
	int currentRow = ui->worldListWidget->currentRow();

	ui->worldListWidget->takeItem(currentRow);

	ui->renameBtn->setEnabled(false);
	ui->deleteBtn->setEnabled(false);

	emit DeleteWorld(currentItem->text());
}

void SelectWorldDialog::ItemActivated(QListWidgetItem *currentItem)
{
	ui->renameBtn->setEnabled(true);
	ui->deleteBtn->setEnabled(true);
}

void SelectWorldDialog::ItemDoubleClicked(QListWidgetItem *currentItem)
{
	hide();
	emit OpenWorld(currentItem->text());
}

void SelectWorldDialog::Rename(QString newWorldID)
{
	QListWidgetItem *currentItem = ui->worldListWidget->currentItem();
	QString oldWorldID = currentItem->text();
	emit RenameWorld(oldWorldID, newWorldID);
	currentItem->setText(newWorldID);
}