#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include "asset.h"
#include "mediaplayer.h"

#include "ui_mainwindow.h"
#include "openglwidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void OpenWorld(QString worldID);

protected:
	virtual void closeEvent(QCloseEvent *evt);

signals:
	void ShowMenu();

private slots:
	void HideAndShowMenu();

private:
	Ui::MainWindowClass ui;

	OpenGLWidget *openglWidget;
	Asset *asset;
	MediaPlayer *mediaPlayer;
};

#endif // MAINWINDOW_H
