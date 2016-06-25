#include "pointlightparamwidget.h"
#include "ui_pointlightparamwidget.h"

#include <QColorDialog>
#include "tool.h"

PointLightParamWidget::PointLightParamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointLightParamWidget)
{
    ui->setupUi(this);

	pl = NULL;

	connect(ui->colorBtn, SIGNAL(clicked()), this, SLOT(ColorBtnClicked()));
	connect(ui->positionXDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(PositionXDoubleSpinBoxValueChanged(double)));
	connect(ui->positionYDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(PositionYDoubleSpinBoxValueChanged(double)));
	connect(ui->positionZDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(PositionZDoubleSpinBoxValueChanged(double)));
	connect(ui->distanceDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DistanceDoubleSpinBoxValueChanged(double)));
	connect(ui->ambientIntensityDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AmbientIntensityDoubleSpinBoxValueChanged(double)));
	connect(ui->diffuseIntensityDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DiffuseIntensityDoubleSpinBoxValueChanged(double)));
	connect(ui->attenuationConstantDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AttenuationConstantDoubleSpinBoxValueChanged(double)));
	connect(ui->attenuationLinearDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AttenuationLinearDoubleSpinBoxValueChanged(double)));
	connect(ui->attenuationExponentDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AttenuationExponentDoubleSpinBoxValueChanged(double)));
}

PointLightParamWidget::~PointLightParamWidget()
{
    delete ui;
}

void PointLightParamWidget::SetTitle(QString title)
{
	ui->pointLightLabel->setText(title);
}

void PointLightParamWidget::SetPointLight(PointLight *pl)
{
	this->pl = pl;

	int r = Clamp((int)(pl->Color.x * 255.0f), 0, 255);
	int g = Clamp((int)(pl->Color.y * 255.0f), 0, 255);
	int b = Clamp((int)(pl->Color.z * 255.0f), 0, 255);

	ui->colorBtn->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));

	ui->positionXDoubleSpinBox->setValue(pl->Position.x);
	ui->positionYDoubleSpinBox->setValue(pl->Position.y);
	ui->positionZDoubleSpinBox->setValue(pl->Position.z);

	ui->distanceDoubleSpinBox->setValue(pl->Distance);

	ui->ambientIntensityDoubleSpinBox->setValue(pl->AmbientIntensity);
	ui->diffuseIntensityDoubleSpinBox->setValue(pl->DiffuseIntensity);

	ui->attenuationConstantDoubleSpinBox->setValue(pl->Attenuation.Constant);
	ui->attenuationLinearDoubleSpinBox->setValue(pl->Attenuation.Linear);
	ui->attenuationExponentDoubleSpinBox->setValue(pl->Attenuation.Exp);
}

void PointLightParamWidget::ColorBtnClicked()
{
	color = QColorDialog::getColor(Qt::white);
	ui->colorBtn->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
	pl->Color.x = color.red() / 255.0f;
	pl->Color.y = color.green() / 255.0f;
	pl->Color.z = color.blue() / 255.0f;
}

void PointLightParamWidget::PositionXDoubleSpinBoxValueChanged(double d)
{
	pl->Position.x = d;
}

void PointLightParamWidget::PositionYDoubleSpinBoxValueChanged(double d)
{
	pl->Position.y = d;
}

void PointLightParamWidget::PositionZDoubleSpinBoxValueChanged(double d)
{
	pl->Position.z = d;
}

void PointLightParamWidget::DistanceDoubleSpinBoxValueChanged(double d)
{
	pl->Distance = d;
}

void PointLightParamWidget::AmbientIntensityDoubleSpinBoxValueChanged(double d)
{
	pl->AmbientIntensity = d;
}

void PointLightParamWidget::DiffuseIntensityDoubleSpinBoxValueChanged(double d)
{
	pl->DiffuseIntensity = d;
}

void PointLightParamWidget::AttenuationConstantDoubleSpinBoxValueChanged(double d)
{
	pl->Attenuation.Constant = d;
}

void PointLightParamWidget::AttenuationLinearDoubleSpinBoxValueChanged(double d)
{
	pl->Attenuation.Linear = d;
}

void PointLightParamWidget::AttenuationExponentDoubleSpinBoxValueChanged(double d)
{
	pl->Attenuation.Exp = d;
}
