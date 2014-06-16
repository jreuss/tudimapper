#include "elementlistwidget.h"
#include "ui_elementlistwidget.h"

ElementListWidget::ElementListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ElementListWidget)
{
    ui->setupUi(this);
    connect(ui->btn_addlayer, SIGNAL(clicked()),
            this, SLOT(handleAddLayer()));

    connect(ui->btn_removelayer, SIGNAL(clicked()),
            this, SLOT(handleRemoveLayer()));

    addLayer = ui->btn_addlayer;
    elementView = ui->treeView_elements;
    scene = NULL;
}

ElementListWidget::~ElementListWidget()
{
    delete ui;
}

void ElementListWidget::setScene(MainScene *scene)
{
    //scene->clearSelection();
    this->scene = scene;
}

void ElementListWidget::setModel(ElementTreeModel *model)
{
    //this->model = model;
    this->elementView->setModel(model);
}

//void ElementListWidget::connectToScene()
//{

//    connect(scene, SIGNAL(selectionChanged()),
//            this,SLOT(handleSceneSelectionChanged()));


//    connect (scene, SIGNAL(onItemDeleted(QList<QGraphicsItem*>)),
//             this, SLOT(handleOnItemsDeleted(QList<QGraphicsItem*>)));

//}

//void ElementListWidget::disconnectScene()
//{
//    if(scene == NULL){
//        return;
//    }

//    disconnect(scene, SIGNAL(selectionChanged()),
//               this,SLOT(handleSceneSelectionChanged()));

//    disconnect (scene, SIGNAL(onItemDeleted(QList<QGraphicsItem*>)),
//                this, SLOT(handleOnItemsDeleted(QList<QGraphicsItem*>)));
//}

QTreeView *ElementListWidget::getElementView() const
{
    return elementView;
}

void ElementListWidget::setElementView(QTreeView *value)
{
    elementView = value;
}

//void ElementListWidget::handleOnItemsDeleted(QList<QGraphicsItem *> itemToRemove)
//{
//    foreach(QGraphicsItem *item, itemToRemove){
//        AbstractTreePixmapItem* i = static_cast<AbstractTreePixmapItem*>(item);
//        static_cast<ElementTreeModel*>(elementView->model())->removeItem(i);
//    }
//}

//void ElementListWidget::handleSceneSelectionChanged()
//{
//    elementView->selectionModel()->clearSelection();
//    foreach(QGraphicsItem *it, scene->selectedItems())
//    {
//        AbstractTreePixmapItem *itm = static_cast<AbstractTreePixmapItem *>(it);
//        QModelIndex index = static_cast<ElementTreeModel*>(elementView->model())->indexFromItem(itm);
//        elementView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
//    }
//}

void ElementListWidget::handleAddLayer()
{
    if(scene == NULL){
        return;
    }
        ItemElement *newLayer = new ItemElement();
        ElementTreeModel *mod = static_cast<ElementTreeModel*>(elementView->model());
        mod->insertItem( mod->getRoot()->getChildren().count(),mod->getRoot(), newLayer);
        newLayer->setParentItem(scene->getRoot());
}

void ElementListWidget::handleRemoveLayer()
{
    if(scene){
        foreach(QModelIndex i,elementView->selectionModel()->selectedIndexes()){
            static_cast<ElementTreeModel*>(elementView->model())->removeItem(i);      }
    }
}

void ElementListWidget::handleMoveLayerUp()
{

}

void ElementListWidget::handleMoveLayerDown()
{

}


