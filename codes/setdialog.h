#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>
#include <QTimer>

#include "uieffect.h"

namespace Ui {
class SetDialog;
}

class SetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetDialog(int initWidth, int initHeight, int fps = 60, QWidget *parent = 0);
    ~SetDialog();

protected:
	virtual void paintEvent(QPaintEvent *evt);
	virtual void resizeEvent(QResizeEvent *evt);

private slots:
	void OKBtnClicked();
	void CancelBtnClicked();

	void MSAAComboBoxCurrentIndexChanged();
	void DisplayModeComboBoxCurrentIndexChanged(int index);

private:
    Ui::SetDialog *ui;

	float deltaTime;
	int initWidth, initHeight;
	QTimer timer;

	UIEffect *uiEffect;
};

#endif // SETDIALOG_H
