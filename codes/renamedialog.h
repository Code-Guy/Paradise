#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>
#include <QTimer>

#include "uieffect.h"

namespace Ui {
class RenameDialog;
}

class RenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(int initWidth, int initHeight, int fps = 60, QWidget *parent = 0);
    ~RenameDialog();

	void SetWorldID(QStringList worldIDs, QString oldWorldID);

protected:
	virtual void paintEvent(QPaintEvent *evt);
	virtual void resizeEvent(QResizeEvent *evt);

signals:
	void Rename(QString newWorldID);

private slots:
	void OKBtnClicked();
	void CancelBtnClicked();

private:
    Ui::RenameDialog *ui;

	QStringList worldIDs;

	float deltaTime;
	int initWidth, initHeight;
	QTimer timer;

	UIEffect *uiEffect;
};

#endif // RENAMEDIALOG_H
