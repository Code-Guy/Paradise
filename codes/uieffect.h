#ifndef _UIEFFECT_H
#define _UIEFFECT_H

#include <QPainter>
#include <QList>

struct UIPolygon
{
	int num;//点数
	float size, bSize, eSize;//大小
	float sizeSpeed;//大小变化的速度
	QPoint pos;//位置（固定不变）
	float angle;//初始旋转角度[0,360)（逆时针）
	float angleSpeed;//旋转速度
	QColor brushColor;//brush颜色
	QColor penColor;//pen颜色
	float penWidth;//pen粗细
	int stepTransparency[4];//四个阶段的透明度
	float transparencyRatio[2];
	int transparency;//当前透明度
	float lifeTime;//寿命
	float curTime;//当前时间
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
	QPoint uiSize;//widget的大小
	float minAngle, maxAngle;
	float minAngleSpeed, maxAngleSpeed;
	QColor minBrushColor, maxBrushColor;
	QColor minPenColor, maxPenColor;
	float minPenWidth, maxPenWidth;
	float minLifeTime, maxLifeTime;
	float emitSpeed;//一秒钟产生多少个粒子
	float changeBgColorTime;

	float changeBgColorTimer;

	float emitNumF;

	QColor backgroundColor;
	QPixmap backgroundPixmap;

	QPointF ratio;

	QPoint Rescale(QPoint pos);
};

#endif //_UIEFFECT_H