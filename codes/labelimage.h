#ifndef _LABELIMAGE_H
#define _LABELIMAGE_H

#include <QLabel>
#include <QPalette>
#include <QGraphicsOpacityEffect>
#include <string>

#define HOVER_IN_ALPHA 0.5

enum ImgType
{
	Construtive = 0, Decorative = 1
};

class LabelIamge : public QLabel
{
	Q_OBJECT

public:
	LabelIamge(std::string imgPath, ImgType imgType, int index);
	~LabelIamge();

protected:
	virtual void enterEvent(QEvent *evt);
	virtual void leaveEvent(QEvent *evt);

signals:
	void Activate(ImgType imgType, int index);

private:
	QGraphicsOpacityEffect *effect;

	ImgType imgType;
	int index;
};

#endif //_LABELIMAGE_H