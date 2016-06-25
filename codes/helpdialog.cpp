#include "helpdialog.h"
#include "ui_helpdialog.h"

#include <QPainter>

HelpDialog::HelpDialog(int initWidth, int initHeight, int fps, QWidget *parent) :
	initWidth(initWidth), initHeight(initHeight), deltaTime(1.0f / fps),
	QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

	setWindowTitle("Help");
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
}

HelpDialog::~HelpDialog()
{
    delete ui;
	delete uiEffect;
}

void HelpDialog::paintEvent(QPaintEvent *evt)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	uiEffect->Logic(deltaTime);
	uiEffect->Render(&painter);
}

void HelpDialog::resizeEvent(QResizeEvent *evt)
{
	QPointF ratio = QPointF(1.0f * width() / initWidth, 1.0f * height() / initHeight);
	uiEffect->Resize(ratio);
}

void HelpDialog::OKBtnClicked()
{
	close();
}