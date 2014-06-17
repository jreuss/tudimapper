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

    connect(ui->btn_moveup, SIGNAL(clicked()),
            this, SLOT(handleMoveLayerUp()));

    connect(ui->btn_movedown, SIGNAL(clicked()),
            this, SLOT(handleMoveLayerDown()));

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

void ElementListWidget::setLayerZIndexes()
{
    int baseZIndex = 10;
    int max = scene->model->getRoot()->childCount();
    QList<AbstractTreePixmapItem*> itms =  scene->model->getRoot()->mChildren;
    for(int i = 0; i < max; i++){
        itms[i]->setZValue(baseZIndex+i);
    }
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
    setLayerZIndexes();
}

void ElementListWidget::handleRemoveLayer()
{
    if(scene){
        foreach(QModelIndex i,elementView->selectionModel()->selectedRows()){
            static_cast<ElementTreeModel*>(elementView->model())->removeItem(i);
        }
        // setLayerZIndexes();
    }
}

void ElementListWidget::handleMoveLayerUp()
{
    int count = elementView->selectionModel()->selectedIndexes().count();

    if(count == 2) {

        QModelIndex index = elementView->selectionModel()->selectedIndexes().front();
        ItemElement *el = static_cast<ItemElement*>(scene->model->itemFromIndex(index));

        if(el->getType() == ItemElement::LAYER) {
            int indexOf = index.row();

            el = static_cast<ItemElement*>(scene->model->takeItem(el));

            scene->model->insertItem(indexOf>0? --indexOf : 0, scene->model->getRoot(), el);
        }
        elementView->selectionModel()->clearSelection();
        elementView->selectionModel()->select(scene->model->indexFromItem(el),
                                              QItemSelectionModel::Select | QItemSelectionModel::Rows);
        setLayerZIndexes();
    }
}

void ElementListWidget::handleMoveLayerDown()
{
    int count = elementView->selectionModel()->selectedIndexes().count();

    if(count == 2) {
        QModelIndex index = elementView->selectionModel()->selectedIndexes().front();
        ItemElement *el = static_cast<ItemElement*>(scene->model->itemFromIndex(index));

        if(el->getType() == ItemElement::LAYER) {
            int indexOf = index.row();
            qDebug() << "c bef.: " << scene->model->getRoot()->childCount();
            el = static_cast<ItemElement*>(scene->model->takeItem(el));
            qDebug() << "c aft.: " << scene->model->getRoot()->childCount() << " will insert at: " << (indexOf<scene->model->getRoot()->childCount()? indexOf+1 : indexOf);
            scene->model->insertItem(indexOf<scene->model->getRoot()->childCount()? ++indexOf : indexOf, scene->model->getRoot(), el);
        }
        elementView->selectionModel()->clearSelection();
        elementView->selectionModel()->select(scene->model->indexFromItem(el),
                                              QItemSelectionModel::Select | QItemSelectionModel::Rows);
        setLayerZIndexes();
    }
}


