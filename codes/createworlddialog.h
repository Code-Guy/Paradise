#ifndef CREATEWORLDDIALOG_H
#define CREATEWORLDDIALOG_H

#include <QDialog>
#include <QTimer>

#include "uieffect.h"

namespace Ui {
class CreateWorldDialog;
}

class CreateWorldDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateWorldDialog(int initWidth, int initHeight, int fps = 60, QWidget *parent = 0);
    ~CreateWorldDialog();

protected:
	virtual void paintEvent(QPaintEvent *evt);
	virtual void resizeEvent(QResizeEvent *evt);
	
signals:
	void CreateWorld(QString worldID);

private slots:
	void OKBtnClicked();
	void CancelBtnClicked();

	void CreateWorldVerify(bool validation);

private:
    Ui::CreateWorldDialog *ui;

	float deltaTime;
	int initWidth, initHeight;
	QTimer timer;

	UIEffect *uiEffect;
};

#endif // CREATEWORLDDIALOG_H
