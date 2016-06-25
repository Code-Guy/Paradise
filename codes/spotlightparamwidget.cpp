#include "spotlightparamwidget.h"
#include "ui_spotlightparamwidget.h"

#include <QColorDialog>
#include "tool.h"

SpotLightParamWidget::SpotLightParamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpotLightParamWidget)
{
    ui->setupUi(this);

	sl = NULL;

	connect(ui->colorBtn, SIGNAL(clicked()), this, SLOT(ColorBtnClicked()));
	connect(ui->positionXDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(PositionXDoubleSpinBoxValueChanged(double)));
	connect(ui->positionYDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(PositionYDoubleSpinBoxValueChanged(double)));
	connect(ui->positionZDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(PositionZDoubleSpinBoxValueChanged(double)));
	connect(ui->distanceDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DistanceDoubleSpinBoxValueChanged(double)));
	connect(ui->cutoffDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(CutoffDoubleSpinBoxValueChanged(double)));
	connect(ui->ambientIntensityDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AmbientIntensityDoubleSpinBoxValueChanged(double)));
	connect(ui->diffuseIntensityDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(DiffuseIntensityDoubleSpinBoxValueChanged(double)));
	connect(ui->attenuationConstantDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AttenuationConstantDoubleSpinBoxValueChanged(double)));
	connect(ui->attenuationLinearDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AttenuationLinearDoubleSpinBoxValueChanged(double)));
	connect(ui->attenuationExponentDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(AttenuationExponentDoubleSpinBoxValueChanged(double)));
}

SpotLightParamWidget::~SpotLightParamWidget()
{
    delete ui;
}

void SpotLightParamWidget::SetTitle(QString title)
{
	ui->spotLightLabel->setText(title);
}

void SpotLightParamWidget::SetSpotLight(SpotLight *sl)
{
	this->sl = sl;

	int r = Clamp((int)(sl->Color.x * 255.0f), 0, 255);
	int g = Clamp((int)(sl->Color.y * 255.0f), 0, 255);
	int b = Clamp((int)(sl->Color.z * 255.0f), 0, 255);

	ui->colorBtn->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b));

	ui->positionXDoubleSpinBox->setValue(sl->Position.x);
	ui->positionYDoubleSpinBox->setValue(sl->Position.y);
	ui->positionZDoubleSpinBox->setValue(sl->Position.z);

	ui->cutoffDoubleSpinBox->setValue(sl->Cutoff);

	ui->ambientIntensityDoubleSpinBox->setValue(sl->AmbientIntensity);
	ui->diffuseIntensityDoubleSpinBox->setValue(sl->DiffuseIntensity);
}

void SpotLightParamWidget::ColorBtnClicked()
{
	color = QColorDialog::getColor(Qt::white);
	ui->colorBtn->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
	sl->Color.x = color.red() / 255.0f;
	sl->Color.y = color.green() / 255.0f;
	sl->Color.z = color.blue() / 255.0f;
}

void SpotLightParamWidget::PositionXDoubleSpinBoxValueChanged(double d)
{
	sl->Position.x = d;
}

void SpotLightParamWidget::PositionYDoubleSpinBoxValueChanged(double d)
{
	sl->Position.y = d;
}

void SpotLightParamWidget::PositionZDoubleSpinBoxValueChanged(double d)
{
	sl->Position.z = d;
}

void SpotLightParamWidget::DistanceDoubleSpinBoxValueChanged(double d)
{
	sl->Distance = d;
}

void SpotLightParamWidget::CutoffDoubleSpinBoxValueChanged(double d)
{
	sl->Cutoff = d;
}

void SpotLightParamWidget::AmbientIntensityDoubleSpinBoxValueChanged(double d)
{
	sl->AmbientIntensity = d;
}

void SpotLightParamWidget::DiffuseIntensityDoubleSpinBoxValueChanged(double d)
{
	sl->DiffuseIntensity = d;
}

void SpotLightParamWidget::AttenuationConstantDoubleSpinBoxValueChanged(double d)
{
	sl->Attenuation.Constant = d;
}

void SpotLightParamWidget::AttenuationLinearDoubleSpinBoxValueChanged(double d)
{
	sl->Attenuation.Linear = d;
}

void SpotLightParamWidget::AttenuationExponentDoubleSpinBoxValueChanged(double d)
{
	sl->Attenuation.Exp = d;
}