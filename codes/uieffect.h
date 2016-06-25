#ifndef _UIEFFECT_H
#define _UIEFFECT_H

#include <QPainter>
#include <QList>

struct UIPolygon
{
	int num;//����
	float size, bSize, eSize;//��С
	float sizeSpeed;//��С�仯���ٶ�
	QPoint pos;//λ�ã��̶����䣩
	float angle;//��ʼ��ת�Ƕ�[0,360)����ʱ�룩
	float angleSpeed;//��ת�ٶ�
	QColor brushColor;//brush��ɫ
	QColor penColor;//pen��ɫ
	float penWidth;//pen��ϸ
	int stepTransparency[4];//�ĸ��׶ε�͸����
	float transparencyRatio[2];
	int transparency;//��ǰ͸����
	float lifeTime;//����
	float curTime;//��ǰʱ��
};

class UIEffect
{
public:
	UIEffect(int minNum, int maxNum,
	float minSize, float maxSize,
	float minSizeSpeed, float maxSizeSpeed,
	QPoint uiSize,
	float minAngle, float maxAngle,
	float minAngleSpeed, float maxAngleSpeed,
	QColor minBrushColor, QColor maxBrushColor,
	QColor minPenColor, QColor maxPenColor,
	float minPenWidth, float maxPenWidth,
	float minLifeTime, float maxLifeTime,
	float emitSpeed,
	float changeBgColorTime);

	~UIEffect();

	void Logic(float deltaTime);
	void Render(QPainter *painter);

	void Resize(QPointF ratio);

private:
	QList<UIPolygon> uiPolygons;

	int minNum, maxNum;
	float minSize, maxSize;
	float minSizeSpeed, maxSizeSpeed;
	QPoint uiSize;//widget�Ĵ�С
	float minAngle, maxAngle;
	float minAngleSpeed, maxAngleSpeed;
	QColor minBrushColor, maxBrushColor;
	QColor minPenColor, maxPenColor;
	float minPenWidth, maxPenWidth;
	float minLifeTime, maxLifeTime;
	float emitSpeed;//һ���Ӳ������ٸ�����
	float changeBgColorTime;

	float changeBgColorTimer;

	float emitNumF;

	QColor backgroundColor;
	QPixmap backgroundPixmap;

	QPointF ratio;

	QPoint Rescale(QPoint pos);
};

#endif //_UIEFFECT_H