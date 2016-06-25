#ifndef SPOTLIGHTPARAMWIDGET_H
#define SPOTLIGHTPARAMWIDGET_H

#include <QWidget>

#include "light.h"

namespace Ui {
class SpotLightParamWidget;
}

class SpotLightParamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpotLightParamWidget(QWidget *parent = 0);
    ~SpotLightParamWidget();

	void SetTitle(QString title);
	void SetSpotLight(SpotLight *sl);

private slots:  
	void ColorBtnClicked();

	void PositionXDoubleSpinBoxValueChanged(double d);
	void PositionYDoubleSpinBoxValueChanged(double d);
	void PositionZDoubleSpinBoxValueChanged(double d);

	void DistanceDoubleSpinBoxValueChanged(double d);

	void CutoffDoubleSpinBoxValueChanged(double d);

	void AmbientIntensityDoubleSpinBoxValueChanged(double d);
	void DiffuseIntensityDoubleSpinBoxValueChanged(double d);

	void AttenuationConstantDoubleSpinBoxValueChanged(double d);
	void AttenuationLinearDoubleSpinBoxValueChanged(double d);
	void AttenuationExponentDoubleSpinBoxValueChanged(double d);

private:
    Ui::SpotLightParamWidget *ui;

	SpotLight *sl;

	QColor color;
};

#endif // SPOTLIGHTPARAMWIDGET_H
