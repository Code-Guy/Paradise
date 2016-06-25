#ifndef POINTLIGHTPARAMWIDGET_H
#define POINTLIGHTPARAMWIDGET_H

#include <QWidget>

#include "light.h"

namespace Ui {
class PointLightParamWidget;
}

class PointLightParamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointLightParamWidget(QWidget *parent = 0);
    ~PointLightParamWidget();

	void SetTitle(QString title);
	void SetPointLight(PointLight *pl);

private slots:
	void ColorBtnClicked();

	void PositionXDoubleSpinBoxValueChanged(double d);
	void PositionYDoubleSpinBoxValueChanged(double d);
	void PositionZDoubleSpinBoxValueChanged(double d);
	void DistanceDoubleSpinBoxValueChanged(double d);
	void AmbientIntensityDoubleSpinBoxValueChanged(double d);
	void DiffuseIntensityDoubleSpinBoxValueChanged(double d);
	void AttenuationConstantDoubleSpinBoxValueChanged(double d);
	void AttenuationLinearDoubleSpinBoxValueChanged(double d);
	void AttenuationExponentDoubleSpinBoxValueChanged(double d);

private:
    Ui::PointLightParamWidget *ui;

	PointLight *pl;

	QColor color;
};

#endif // POINTLIGHTPARAMWIDGET_H
