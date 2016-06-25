#ifndef _PACKAGEDIALOG_H
#define _PACKAGEDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <vector>
#include <string>
#include "labelimage.h"

#define ROW 3
#define PACKAGE_DIALOG_WIDTH 310
#define PACKAGE_DIALOG_HEIGHT 310

class PackageDialog : public QDialog
{
	Q_OBJECT

public:
	PackageDialog(QWidget *parent = 0);
	~PackageDialog();

protected:
	virtual void keyPressEvent(QKeyEvent *evt);

signals:
	void SetShortcut(int slotIndex, std::string imgPath, std::string meshPath);

private slots:
	void Activate(ImgType imgType, int index);

private:
	QTabWidget *tabWidget;
	QScrollArea *cbScrollArea, *dbScrollArea;
	QWidget *cbWidget, *dbWidget;
	QGridLayout *cbGridLayout, *dbGridLayout;
	int cbCount, dbCount;

	QVBoxLayout *vLayout;

	ImgType activatedImgType;
	int activatedIndex;

	std::vector<std::string> constructiveImgPaths;
	std::vector<std::string> decorativeImgPaths;

	void AddCB(std::string imgPath);
	void AddDB(std::string imgPath);
};

#endif //_PACKAGEDIALOG_H