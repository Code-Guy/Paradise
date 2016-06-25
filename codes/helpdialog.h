#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QTimer>

#include "uieffect.h"

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(int initWidth, int initHeight, int fps = 60, QWidget *parent = 0);
    ~HelpDialog();

protected:
	virtual void paintEvent(QPaintEvent *evt);
	virtual void resizeEvent(QResizeEvent *evt);

private slots:
	void OKBtnClicked();

private:
    Ui::HelpDialog *ui;

	float deltaTime;
	int initWidth, initHeight;
	QTimer timer;

	UIEffect *uiEffect;
};

#endif // HELPDIALOG_H
