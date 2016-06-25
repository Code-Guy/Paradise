#ifndef DIRECTIONALLIGHTPARAMWIDGET_H
#define DIRECTIONALLIGHTPARAMWIDGET_H

#include <QWidget>
#include "light.h"

namespace Ui {
class DirectionalLightParamWidget;
}

class DirectionalLightParamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DirectionalLightParamWidget(QWidget *parent = 0);
    ~DirectionalLightParamWidget();

	void SetDirectionalLight(DirectionalLight *dl);

private slots:
	void ColorBtnClicked();

	void DirectionXDoubleSpinBoxValueChanged(double d);
	void DirectionYDoubleSpinBoxValueChanged(double d);
	void DirectionZDoubleSpinBoxValueChanged(double d);
	void AmbientIntensityDoubleSpinBoxValueChanged(double d);
	void DiffuseIntensityDoubleSpinBoxValueChanged(double d);

private:
    Ui::DirectionalLightParamWidget *ui;

	DirectionalLight *dl;

	QColor color;
};

#endif // DIRECTIONALLIGHTPARAMWIDGET_H
