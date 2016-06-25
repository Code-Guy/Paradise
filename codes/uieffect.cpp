#include "uieffect.h"
#include "tool.h"
#include <qmath.h>

UIEffect::UIEffect(int minNum, int maxNum,
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
	float changeBgColorTime)
	: minNum(minNum), maxNum(maxNum),
	minSize(minSize), maxSize(maxSize),
	minSizeSpeed(minSizeSpeed), maxSizeSpeed(maxSizeSpeed),
	uiSize(uiSize),
	minAngle(minAngle), maxAngle(maxAngle),
	minAngleSpeed(minAngleSpeed), maxAngleSpeed(maxAngleSpeed),
	minBrushColor(minBrushColor), maxBrushColor(maxBrushColor),
	minPenColor(minPenColor), maxPenColor(maxPenColor),
	minPenWidth(minPenWidth), maxPenWidth(maxPenWidth),
	minLifeTime(minLifeTime), maxLifeTime(maxLifeTime),
	emitSpeed(emitSpeed),
	changeBgColorTime(changeBgColorTime)
{
	emitNumF = 0;
	ratio = QPointF(1.0f, 1.0f);
	changeBgColorTimer = 0;
	backgroundColor = QColor(255, 255, 255);
	backgroundPixmap = QPixmap("Resources/texture/ui_background.png");
}

UIEffect::~UIEffect()
{

}

void UIEffect::Logic(float deltaTime)
{
	/**emit**/
	emitNumF += emitSpeed * deltaTime;
	int emitNumI = (int)emitNumF;
	if (emitNumF >= 1.0f)
	{
		emitNumF -= emitNumI;
	}

	for (int i=0; i<emitNumI; i++)
	{
		UIPolygon polygon;
		polygon.num = RandIntToInt(minNum, maxNum);
		polygon.bSize = RandFloatToFloat(minSize, 1.2f * minSize);
		polygon.eSize = RandFloatToFloat(0.8f * maxSize, maxSize);
		polygon.size = polygon.bSize;
		polygon.sizeSpeed = RandFloatToFloat(minSizeSpeed, maxSizeSpeed);
		polygon.pos = RandPos(uiSize);
		polygon.angle = RandFloatToFloat(minAngle, maxAngle);
		polygon.angleSpeed = RandFloatToFloat(minAngleSpeed, maxAngleSpeed);
		polygon.brushColor = RandColorToColor(minBrushColor, maxBrushColor);
		polygon.penColor = RandColorToColor(minPenColor, maxPenColor);
		polygon.penWidth = RandFloatToFloat(minPenWidth, maxPenWidth);
		polygon.stepTransparency[0] = polygon.stepTransparency[3] = 0;
		polygon.stepTransparency[1] = RandIntToInt(25, 225);
		polygon.stepTransparency[2] = RandIntToInt(25, 225);
		polygon.transparencyRatio[0] = RandFloatToFloat(0.1, 0.4);
		polygon.transparencyRatio[1] = RandFloatToFloat(0.6, 0.9);
		polygon.lifeTime = RandFloatToFloat(minLifeTime, maxLifeTime);
		polygon.curTime = 0;
		uiPolygons.append(polygon);
	}

	/**update**/
	for (QList<UIPolygon>::iterator iter = uiPolygons.begin(); iter != uiPolygons.end(); )
	{
		//lifetime
		iter->curTime += deltaTime;
		if (iter->curTime > iter->lifeTime)
		{
			iter = uiPolygons.erase(iter);
			continue;
		}
		float lifeRatio = iter->curTime / iter->lifeTime;
		//size
		if (iter->size < iter->bSize || iter->size > iter->eSize)
		{
			iter->sizeSpeed *= -1;
		}
		iter->size += iter->sizeSpeed * deltaTime;
		//angle
		iter->angle += iter->angleSpeed * deltaTime;
		if (iter->angle > 360)
		{
			iter->angle -= 360;
		}
		//transparency
		if (lifeRatio <= iter->transparencyRatio[0])
		{
			iter->transparency = iter->stepTransparency[0] + 
				(iter->stepTransparency[1] - iter->stepTransparency[0]) * 
				lifeRatio / iter->transparencyRatio[0];
		}
		else if (lifeRatio > iter->transparencyRatio[0] && lifeRatio <= iter->transparencyRatio[1])
		{
			iter->transparency = iter->stepTransparency[1] + 
				(iter->stepTransparency[2] - iter->stepTransparency[1]) * 
				(lifeRatio - iter->transparencyRatio[0]) / (iter->transparencyRatio[1] - iter->transparencyRatio[0]);
		}
		else 
		{
			iter->transparency = iter->stepTransparency[2] + 
				(iter->stepTransparency[3] - iter->stepTransparency[2]) * 
				(lifeRatio - iter->transparencyRatio[1]) / (1 - iter->transparencyRatio[1]);
		}
		iter->penColor.setAlpha(iter->transparency);
		iter->brushColor.setAlpha(iter->transparency);
		iter++;
	}
	/**change background color**/
	changeBgColorTimer += deltaTime;
	if (changeBgColorTimer > changeBgColorTime)
	{
		backgroundColor = RandColorToColor(QColor(100, 100, 100, 0), QColor(255, 255, 255, 0));
		changeBgColorTimer = 0;
	}
	else
	{
		backgroundColor.setAlpha(255 * qSin(3.1415926f * changeBgColorTimer / changeBgColorTime));
	}
}

void UIEffect::Render(QPainter *painter)
{
	/**draw background**/
	painter->setBrush(backgroundColor);
	painter->drawRect(-1, -1, uiSize.x() * ratio.rx() + 5, uiSize.y() * ratio.ry() + 5);

	/**draw polygons**/
	for (QList<UIPolygon>::const_iterator iter = uiPolygons.begin(); iter != uiPolygons.end(); iter++)
	{
		painter->setPen(QPen(iter->penColor, iter->penWidth));
		painter->setBrush(iter->brushColor);

		QPolygon polygon;
		for (int i=0; i<iter->num; i++)
		{
			float angle = qDegreesToRadians(360.0f * i / iter->num + iter->angle);
			int x = iter->size * qCos(angle);
			int y = iter->size * qSin(angle);

			polygon.append(Rescale(iter->pos + QPoint(x, y)));
		}
		painter->drawPolygon(polygon);
	}
	/**draw background pixmap**/
	painter->drawPixmap(-1, -1, uiSize.x() * ratio.rx() + 5, uiSize.y() * ratio.ry() + 5, backgroundPixmap);
}

void UIEffect::Resize(QPointF ratio)
{
	this->ratio = ratio;
}

QPoint UIEffect::Rescale(QPoint pos)
{
	return QPoint(pos.x() * ratio.rx(), pos.y() * ratio.ry());
}