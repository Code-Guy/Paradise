#ifndef SELECTWORLDDIALOG_H
#define SELECTWORLDDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QTimer>

#include "uieffect.h"
#include "renamedialog.h"
#include "createworlddialog.h"

namespace Ui {
class SelectWorldDialog;
}

class SelectWorldDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectWorldDialog(int initWidth, int initHeight, int fps = 60, QWidget *parent = 0);
    ~SelectWorldDialog();

	void InitListWidget(QStringList worldIDs);

protected:
	virtual void paintEvent(QPaintEvent *evt);
	virtual void resizeEvent(QResizeEvent *evt);

signals:
	void CreateWorldVerify(bool validation);
	void CreateWorld(QString worldID);
	void DeleteWorld(QString worldID);
	void OpenWorld(QString worldID);
	void RenameWorld(QString oldWorldID, QString newWorldID);

private slots:
	void CreateBtnClicked();
	void RenameClicked();
	void DeleteBtnClicked();

	void ItemActivated(QListWidgetItem *);
	void ItemDoubleClicked(QListWidgetItem *);

	void Rename(QString newWorldID);

private:
    Ui::SelectWorldDialog *ui;

	float deltaTime;
	int initWidth, initHeight;
	QTimer timer;

	UIEffect *uiEffect;

	RenameDialog *renameDialog;
	CreateWorldDialog *createWorldDialog;
};

#endif // SELECTWORLDDIALOG_H
