#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QTimer>

#include "mainwindow.h"
#include "selectworlddialog.h"
#include "setdialog.h"
#include "helpdialog.h"

#include "uieffect.h"

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(int initWidth, int initHeight, int fps = 60, QWidget *parent = 0);
    ~Menu();

protected:
	virtual void paintEvent(QPaintEvent *evt);
	virtual void closeEvent(QCloseEvent *evt);
	virtual void resizeEvent(QResizeEvent *evt);

signals:
	void CreateWorldVerify(bool validation);

private slots:
	void SelectWorldBtnClicked();
	void SetBtnClicked();
	void HelpBtnClicked();
	void ExitBtnClicked();

	void CreateWorld(QString worldID);
	void DeleteWorld(QString worldID);
	void OpenWorld(QString worldID);
	void RenameWorld(QString oldWorldID, QString newWorldID);

	void ShowMenu();

private:
    Ui::Menu *ui;

	float deltaTime;
	int initWidth, initHeight;
	QTimer timer;

	QStringList worldIDs;

	MainWindow *mainWindow;
	SelectWorldDialog *selectWorldDialog;
	SetDialog *setDialog;
	HelpDialog *helpDialog;

	UIEffect *uiEffect;
};

#endif // MENU_H
