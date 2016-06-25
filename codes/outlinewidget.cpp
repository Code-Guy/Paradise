#include "outlinewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidgetItem>
#include <QList>
#include <QDebug>
#include <QAction>
#include <QMenu>

#include "asset.h"

using namespace std;

OutlineWidget::OutlineWidget(QWidget* parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::WindowStaysOnTopHint);

	treeWidget = new QTreeWidget;
	treeWidget->setHeaderLabel("Outline");
	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	stackedWidget = new QStackedWidget;
	stackedWidget->setMinimumWidth(100);
	
	dlpw = new DirectionalLightParamWidget;
	plpw = new PointLightParamWidget;
	slpw = new SpotLightParamWidget;

	stackedWidget->addWidget(new QWidget);
	stackedWidget->addWidget(dlpw);
	stackedWidget->addWidget(plpw);
	stackedWidget->addWidget(slpw);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(treeWidget);
	hLayout->addSpacing(10);
	hLayout->addWidget(stackedWidget);
	mainLayout->addLayout(hLayout);

	setLayout(mainLayout);

	connect(treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), 
		this, SLOT(CurrentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
	connect(treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), 
		this, SLOT(PrepareContextMenu(const QPoint &)));
}

OutlineWidget::~OutlineWidget()
{
	delete treeWidget;
	delete stackedWidget;
}

void OutlineWidget::Init(const std::list<Block> *blocks, 
	DirectionalLight *dl, std::vector<PointLight> *pls, std::vector<SpotLight> *sls)
{
	this->blocks = blocks;
	this->dl = dl;
	this->pls = pls;
	this->sls = sls;

	treeWidget->clear();

	QTreeWidgetItem *blockItem = new QTreeWidgetItem;
	blockItem->setText(0, "Block");
	blockItem->setIcon(0, QIcon(QPixmap("Resources/texture/default_texture_name.png").scaled(ITEM_ICON_SIZE, ITEM_ICON_SIZE)));

	QTreeWidgetItem *lightItem = new QTreeWidgetItem;
	lightItem->setText(0, "Light");
	lightItem->setIcon(0, QIcon(QPixmap("Resources/texture/light.png").scaled(ITEM_ICON_SIZE, ITEM_ICON_SIZE)));

	treeWidget->addTopLevelItem(blockItem);
	treeWidget->addTopLevelItem(lightItem);

	QTreeWidgetItem *directionalLightItem = new QTreeWidgetItem;
	directionalLightItem->setText(0, "Directional Light");
	directionalLightItem->setIcon(0, QIcon(QPixmap("Resources/texture/directional_light.png").scaled(ITEM_ICON_SIZE, ITEM_ICON_SIZE)));

	pointLightsItem = new QTreeWidgetItem;
	pointLightsItem->setText(0, "Point Lights");
	pointLightsItem->setIcon(0, QIcon(QPixmap("Resources/texture/point_light.png").scaled(ITEM_ICON_SIZE, ITEM_ICON_SIZE)));
	for (int i=0; i<pls->size(); i++)
	{
		QTreeWidgetItem *pointLightItem = new QTreeWidgetItem;
		pointLightItem->setText(0, QString::number(i));
		pointLightsItem->addChild(pointLightItem);
	}

	spotLightsItem = new QTreeWidgetItem;
	spotLightsItem->setText(0, "Spot Lights");
	spotLightsItem->setIcon(0, QIcon(QPixmap("Resources/texture/spot_light.png").scaled(ITEM_ICON_SIZE, ITEM_ICON_SIZE)));
	for (int i=0; i<sls->size(); i++)
	{
		QTreeWidgetItem *spotLightItem = new QTreeWidgetItem;
		spotLightItem->setText(0, QString::number(i));
		spotLightsItem->addChild(spotLightItem);
	}

	treeWidget->topLevelItem(1)->addChild(directionalLightItem);
	treeWidget->topLevelItem(1)->addChild(pointLightsItem);
	treeWidget->topLevelItem(1)->addChild(spotLightsItem);

	treeWidget->expandAll();
}

void OutlineWidget::Update()
{
	for (list<Block>::const_iterator bi = blocks->begin(); bi != blocks->end(); bi++)
	{
		QString blockTexturePath = QString::fromStdString(Asset::GetInstance()->GetTexturePath(bi->GetTexture()));
		QString blockTextureName = Asset::GetInstance()->GetTextureName(bi->GetTexture());

		QTreeWidgetItem *blockItem = FindBlockItem(blockTextureName);

		if(blockItem != NULL)
		{
			blockItem->child(0)->setText(0, QString("%1").arg(bi->cells.size()));
		}
		else
		{
			QTreeWidgetItem *item = new QTreeWidgetItem;
			item->setText(0, blockTextureName);
			item->setIcon(0, QIcon(QPixmap(blockTexturePath).scaled(ITEM_ICON_SIZE, ITEM_ICON_SIZE)));

			QTreeWidgetItem *childItem = new QTreeWidgetItem;
			childItem->setText(0, QString("%1").arg(bi->cells.size()));

			item->addChild(childItem);

			treeWidget->topLevelItem(0)->addChild(item);
		}
	}
}

void OutlineWidget::keyPressEvent(QKeyEvent *evt)
{
	switch (evt->key())
	{
	case Qt::Key_Escape:
		hide();
		break;
	}
}

void OutlineWidget::CurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if (current != NULL)
	{
		if (current->text(0) == "Directional Light")//Directional Lights
		{
			dlpw->SetDirectionalLight(dl);
			stackedWidget->setCurrentWidget(dlpw);
		}
		else if (current->parent() != NULL)
		{
			if (current->parent()->text(0) == "Point Lights")//Point Lights
			{
				plpw->SetTitle(QString("Point Light %1").arg(current->text(0)));
				plpw->SetPointLight(&(pls->at(current->text(0).toInt())));
				stackedWidget->setCurrentWidget(plpw);
			}
			else if (current->parent()->text(0) == "Spot Lights")//Spot Lights
			{
				slpw->SetTitle(QString("Spot Light %1").arg(current->text(0)));
				slpw->SetSpotLight(&(sls->at(current->text(0).toInt())));
				stackedWidget->setCurrentWidget(slpw);
			}
		}
	}
}

void OutlineWidget::PrepareContextMenu(const QPoint &pos)
{
	QTreeWidgetItem *curItem = treeWidget->itemAt(pos);

	if (curItem != NULL)
	{
		if (curItem->text(0) == "Point Lights")
		{
			QAction *action = new QAction("Add", this);
			connect(action, SIGNAL(triggered()), this, SLOT(AddPointLight()));
			QMenu menu(this);
			menu.addAction(action);
			menu.exec(treeWidget->mapToGlobal(pos));
		}
		else if (curItem->text(0) == "Spot Lights")
		{
			QAction *action = new QAction("Add", this);
			connect(action, SIGNAL(triggered()), this, SLOT(AddSpotLight()));
			QMenu menu(this);
			menu.addAction(action);
			menu.exec(treeWidget->mapToGlobal(pos));
		}
		else if (curItem->parent() != NULL)
		{
			if (curItem->parent()->text(0) == "Point Lights")
			{
				deleteLightItem = curItem;
				QAction *action = new QAction("Delete", this);
				connect(action, SIGNAL(triggered()), this, SLOT(DeletePointLight()));
				QMenu menu(this);
				menu.addAction(action);
				menu.exec(treeWidget->mapToGlobal(pos));
			}
			else if (curItem->parent()->text(0) == "Spot Lights")
			{
				deleteLightItem = curItem;
				QAction *action = new QAction("Delete", this);
				connect(action, SIGNAL(triggered()), this, SLOT(DeleteSpotLight()));
				QMenu menu(this);
				menu.addAction(action);
				menu.exec(treeWidget->mapToGlobal(pos));
			}	
		}
	}
}

void OutlineWidget::AddPointLight()
{
	pls->push_back(PointLight());
	QTreeWidgetItem *pointLightItem = new QTreeWidgetItem;
	pointLightItem->setText(0, QString::number(pls->size() - 1));
	pointLightsItem->addChild(pointLightItem);
}

void OutlineWidget::AddSpotLight()
{
	sls->push_back(SpotLight());
	QTreeWidgetItem *spotLightItem = new QTreeWidgetItem;
	spotLightItem->setText(0, QString::number(sls->size() - 1));
	spotLightsItem->addChild(spotLightItem);
}

void OutlineWidget::DeletePointLight()
{
	int index = deleteLightItem->text(0).toInt();
	pls->erase(pls->begin() + index);
	deleteLightItem->parent()->takeChild(index);
}

void OutlineWidget::DeleteSpotLight()
{
	int index = deleteLightItem->text(0).toInt();
	sls->erase(sls->begin() + index);
	deleteLightItem->parent()->takeChild(index);
}

QTreeWidgetItem *OutlineWidget::FindBlockItem(QString blockTextureName)
{
	QTreeWidgetItem *blocksItem = treeWidget->topLevelItem(0);
	for (int i=0; i<blocksItem->childCount(); i++)
	{
		QTreeWidgetItem *child = blocksItem->child(i);
		if (child->text(0) == blockTextureName)
		{
			return child;
		}
	}
	return NULL;
}