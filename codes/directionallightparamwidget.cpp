#include "directionallightparamwidget.h"
#include "ui_directionallightparamwidget.h"
#include "tool.h"
#include <QDebug>
#include <QColorDialog>

DirectionalLightParamWidget::DirectionalLightParamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DirectionalLightParamWidget)
{
    ui->setupUi(this);

	dl = NULL;

	connect(ui->colorBtn, SIGNAL(clicked()), this, SLOT(ColorBtnClicked()));
	connect(ui->directionXDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DirectionXDoubleSpinBoxValueChanged(double)));
	connect(ui->directionYDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DirectionYDoubleSpinBoxValueChanged(double)));
	connect(ui->directionZDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DirectionZDoubleSpinBoxValueChanged(double)));
	connect(ui->ambientIntensityDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AmbientIntensityDoubleSpinBoxValueChanged(double)));
	connect(ui->diffuseIntensityDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DiffuseIntensityDoubleSpinBoxValueChanged(double)));
}

DirectionalLightParamWidget::~DirectionalLightParamWidget()
{
    delete ui;
}

void DirectionalLightParamWidget::SetDirectionalLight(DirectionalLight *dl)
{
	this->dl = dl;

	int r = Clamp((int)(dl->Color.x * 255.0f), 0, 255);
	int g = Clamp((int)(dl->Color.y * 255.0f), 0, 255);
	int b = Clamp((int)(dl->Color.z * 255.0f), 0, 255);

	ui->colorBtn->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));

	ui->directionXDoubleSpinBox->setValue(dl->Direction.x);
	ui->directionYDoubleSpinBox->setValue(dl->Direction.y);
	ui->directionZDoubleSpinBox->setValue(dl->Direction.z);

	ui->ambientIntensityDoubleSpinBox->setValue(dl->AmbientIntensity);
	ui->diffuseIntensityDoubleSpinBox->setValue(dl->DiffuseIntensity);
}

void DirectionalLightParamWidget::ColorBtnClicked()
{
	color = QColorDialog::getColor(Qt::white);
	ui->colorBtn->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
	dl->Color.x = color.red() / 255.0f;
	dl->Color.y = color.green() / 255.0f;
	dl->Color.z = color.blue() / 255.0f;
}

void DirectionalLightParamWidget::DirectionXDoubleSpinBoxValueChanged(double d)
{
	dl->Direction.x = d;
}

void DirectionalLightParamWidget::DirectionYDoubleSpinBoxValueChanged(double d)
{
	dl->Direction.y = d;
}

void DirectionalLightParamWidget::DirectionZDoubleSpinBoxValueChanged(double d)
{
	dl->Direction.z = d;
}

void DirectionalLightParamWidget::AmbientIntensityDoubleSpinBoxValueChanged(double d)
{
	dl->AmbientIntensity = d;
}

void DirectionalLightParamWidget::DiffuseIntensityDoubleSpinBoxValueChanged(double d)
{
	dl->DiffuseIntensity = d;
}