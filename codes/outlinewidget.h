#ifndef _OUTLINEWIDGET_H
#define _OUTLINEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QStackedWidget>
#include <vector>
#include <list>

#include "block.h"
#include "light.h"
#include "directionallightparamwidget.h"
#include "pointlightparamwidget.h"
#include "spotlightparamwidget.h"

#define ITEM_ICON_SIZE 32

class OutlineWidget : public QWidget
{
	Q_OBJECT;

public:
	OutlineWidget(QWidget* parent = 0);
	~OutlineWidget();

	void Init(const std::list<Block> *blocks, 
		DirectionalLight *dl, std::vector<PointLight> *pls, std::vector<SpotLight> *sls);
	void Update();

protected:
	virtual void keyPressEvent(QKeyEvent *evt);

private slots:
	void CurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void PrepareContextMenu(const QPoint &pos);
	void AddPointLight();
	void AddSpotLight();
	void DeletePointLight();
	void DeleteSpotLight();
private:
	QTreeWidget *treeWidget;
	QStackedWidget *stackedWidget;
	
	DirectionalLightParamWidget *dlpw;
	PointLightParamWidget *plpw;
	SpotLightParamWidget *slpw;

	const std::list<Block> *blocks;

	/*ÈýÖÖµÆ¹â*/
	DirectionalLight *dl;
	std::vector<PointLight> *pls;
	std::vector<SpotLight> *sls;

	QTreeWidgetItem *pointLightsItem;
	QTreeWidgetItem *spotLightsItem;
	QTreeWidgetItem *deleteLightItem;
	
	QTreeWidgetItem *FindBlockItem(QString blockTextureName);
};

#endif //_OUTLINEWIDGET_H