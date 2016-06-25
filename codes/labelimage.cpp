#include "labelimage.h"
#include "global.h"

LabelIamge::LabelIamge(std::string imgPath, ImgType imgType, int index) 
	: imgType(imgType), index(index)
{
	setAttribute(Qt::WA_Hover, true);

	setFixedSize(LABEL_IMAGE_SIZE, LABEL_IMAGE_SIZE);
	setPixmap(QPixmap(imgPath.c_str()).scaled(QSize(LABEL_IMAGE_SIZE, LABEL_IMAGE_SIZE)));

	effect = new QGraphicsOpacityEffect;
}

LabelIamge::~LabelIamge()
{

}

void LabelIamge::enterEvent(QEvent *evt)
{
	effect->setOpacity(HOVER_IN_ALPHA);
	setGraphicsEffect(effect);

	emit Activate(imgType, index);

	return QLabel::enterEvent(evt);
}

void LabelIamge::leaveEvent(QEvent *evt)
{
	effect->setOpacity(1.0f);
	setGraphicsEffect(effect);

	return QLabel::leaveEvent(evt);
}