#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    selectedLevel = NULL;
    elementModel = new ElementTreeModel(1);
    //elementModel->setRoot(mainScene->getRoot());

    ui->element_tree->elementView->setModel(elementModel);
    ui->element_tree->elementView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    levelModel = new LevelTreeModel(1);
    ui->treeView_level->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView_level->setModel(levelModel);
    //ui->graphicsView->setScene(mainScene);
    ui->graphicsView->setAcceptDrops(true);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    //<<<<<<< HEAD
    //=======

    // create the layout widget
    layoutWidget = new LayoutWidget(this);
    // restore previous layout
    this->restoreState(settings.value("CURRENT_LAYOUT").toByteArray(), 0);
    // load saved layouts to menu
    loadLayouts();

    //>>>>>>> fefa3362cebb8d17a41531cf6ec91c73d0694306
    createConnections();
}

MainWindow::~MainWindow()
{
    // save the current layout
    settings.setValue("DOCK_LOCATIONS", this->saveState(0));

    delete ui;
}

void MainWindow::createConnections()
{
    connect(layoutWidget, SIGNAL(accepted()),
            this, SLOT(handleUpdateLayoutMenu()));

    connect (ui->actionSave_layout, SIGNAL(triggered()),
             this, SLOT(handleAddNewLayout()));

    connect (ui->actionQuit, SIGNAL (triggered()),
             qApp, SLOT (quit()));
    connect (ui->actionImportSpecial, SIGNAL(triggered()),
             this, SLOT (handleImportSpecial()));

    connect(ui->dockWidgetContents, SIGNAL(onSendRequestedTemplates(QPointF,QList<ItemTemplate*>)),
            this,SLOT(handleTemplatesRecieved(QPointF,QList<ItemTemplate*>)));

    connect(ui->mainToolbar, SIGNAL(onColToggled(bool)),
            this,SLOT(handleShowCollider(bool)));

    connect(ui->mainToolbar, SIGNAL(onScaleToggled(bool)),
            this,SLOT(handleScaleToggled(bool)));

    connect(ui->mainToolbar, SIGNAL(onRotateToggled(bool)),
            this,SLOT(handleRotateToggled(bool)));

            connect (ui->element_tree->elementView, SIGNAL(clicked(QModelIndex)),
                     this, SLOT(handleTreeviewSelectionChanged(QModelIndex)));

    connect (ui->element_tree->elementView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this, SLOT(handleUpdatePropeties(QItemSelection,QItemSelection)));

    connect(ui->spinBox_xpos,SIGNAL(valueChanged(int)),
            this,SLOT(handleChangeXPos(int)));

    connect(ui->spinBox_ypos,SIGNAL(valueChanged(int)),
            this,SLOT(handleChangeYPos(int)));

    connect(ui->spinBox_scale,SIGNAL(valueChanged(double)),
            this,SLOT(handleChangeScale(double)));

    connect(ui->spinBox_rotation,SIGNAL(valueChanged(double)),
            this,SLOT(handleChangeRotation(double)));

    connect(ui->btn_collider,SIGNAL(clicked()),
            this,SLOT(handleUpdateImportOptions()));

    connect (ui->treeView_level->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this, SLOT(handleLevelChange(QItemSelection,QItemSelection)));

    connect(ui->btn_addLevel,SIGNAL(clicked()),
            this, SLOT(handleAddLevel()));
}

void MainWindow::handleImportSpecial()
{
    QList<QUrl> urls = mFileManager.FileDialog();

    if (urls.count() != 0)
    {
        //mImportDialog
        ImportDialog *diag = new ImportDialog(urls, this);

        qDebug() << "theres a skede in the net- contact your local ISP!";
        connect(diag, SIGNAL(onImportAccept(ItemTemplate*)),
                this, SLOT(handleImportAccepted(ItemTemplate*)));

        diag->setModal(true);
        diag->setWindowFlags(Qt::Window);
        diag->exec();

        disconnect(diag, SIGNAL(onImportAccept(ItemTemplate*)),
                   this, SLOT(handleImportAccepted(ItemTemplate*)));

        delete diag;
    }
}

void MainWindow::handleImportAccepted(ItemTemplate *item)
{

    ui->dockWidgetContents->addTemplates(item);
}

int skede = 12;
int fisse = 15;
void MainWindow::handleTemplatesRecieved(QPointF pos, QList<ItemTemplate *> templates)
{
      ItemElement *layer;
    if(ui->element_tree->elementView->selectionModel()->
            selectedIndexes().count() > 0){
            QModelIndex idx = ui->element_tree->elementView->selectionModel()->
                    selectedIndexes().front();
            AbstractTreePixmapItem *temp = elementModel->itemFromIndex(idx);

            ItemElement* selected = static_cast<ItemElement*>(temp);
            if(selected->getType() == ItemElement::LAYER){
                layer = selected;
            } else {
                layer = static_cast<ItemElement*>(selected->getParent());
            }
    } else {

        layer = new ItemElement();
        elementModel->insertItem(elementModel->getRoot()->getChildren().count(),elementModel->getRoot(),layer);
        layer->setParentItem(selectedLevel->getRoot());
    }

    foreach(ItemTemplate* item, templates){
        if(item->importType() == ItemTemplate::Split){
            QList<QGraphicsItem*> elements = item->getSplitScene()->items();
            foreach(QGraphicsItem* currentItem, elements){
                ItemElement *element = new ItemElement(static_cast<ItemElement*>(currentItem));
                element->setParentItem(layer);
                elementModel->insertItem(layer->getChildren().count(), layer, element);

                element->setPos(currentItem->pos());
                element->setScale(currentItem->scale());
            }

        } else {
            ItemElement *element = new ItemElement(item);


            element->setParentItem(layer);
            elementModel->insertItem(layer->getChildren().count(), layer, element);

            element->setPos(pos - QPointF(element->getRect().width()/2, element->getRect().height()/2));
        }
    }

    qDebug() << selectedLevel->childCount();
}

void MainWindow::handleAddNewLayout()
{
    layoutWidget->show();
}

void MainWindow::handleUpdateLayoutMenu()
{
    loadLayouts();
}

void MainWindow::handleRestoreLayout()
{
    QString name = static_cast<QAction*>(QObject::sender())->text();

    for(int i=0; i<layouts.count(); ++i) {
        if(layouts[i].first == name) {
            this->restoreState(layouts[i].second, 0);
            return;
        }
    }
}

void MainWindow::handleSceneSelectionChanged()
{   ui->element_tree->elementView->selectionModel()->clearSelection();
    foreach(QGraphicsItem *it, selectedLevel->selectedItems())
    {
        AbstractTreePixmapItem *itm = static_cast<AbstractTreePixmapItem *>(it);
        QModelIndex index = elementModel->indexFromItem(itm);
        ui->element_tree->elementView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void MainWindow::handleTreeviewSelectionChanged(QModelIndex pressedOn)
{
    Q_UNUSED(pressedOn);

    QModelIndexList idxList = ui->element_tree->elementView->selectionModel()->selectedIndexes();
    foreach(QGraphicsItem *itm, selectedLevel->selectedItems ()){
        itm->setSelected (false);
    }
    foreach (QModelIndex idx , idxList) {

        AbstractTreePixmapItem* temp = elementModel->itemFromIndex(idx);
        ItemElement* itm = static_cast<ItemElement*>(temp);
        if(itm->getType() == ItemElement::LAYER){
            foreach (AbstractTreePixmapItem* c, itm->getChildren()) {
               QModelIndex tempIdx = elementModel->indexFromItem(c);
               ui->element_tree->elementView->selectionModel()->select(tempIdx,QItemSelectionModel::Select | QItemSelectionModel::Rows);
               c->setSelected(true);
            }
        } else {
                itm->setSelected(true);
            }
        }

    }


void MainWindow::handleOnItemsDeleted(QList<QGraphicsItem *> itemToRemove)
{
    foreach(QGraphicsItem *item, itemToRemove){
        AbstractTreePixmapItem* i = static_cast<AbstractTreePixmapItem*>(item);
        elementModel->removeItem(i);
    }
}

void MainWindow::handleUpdatePropeties(QItemSelection seleceted, QItemSelection deselected)
{
    //previousSelectedItems = selectedItems;
    selectedItems.clear();
    QModelIndexList selectedIndexes = ui->element_tree->elementView->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, selectedIndexes){
        selectedItems.append(static_cast<ItemElement*>(elementModel->itemFromIndex(index)));
    }

    if(selectedIndexes.count() == 0){
        ui->stackedWidget->setCurrentIndex(0);
    } else if(selectedItems.count() == 1){
        ui->stackedWidget->setCurrentIndex(1);

        ui->spinBox_rotation->setValue(selectedItems.front()->rotation());
        ui->spinBox_scale->setValue(selectedItems.front()->transform().m11());
        ui->spinBox_xpos->setValue(selectedItems.front()->pos().x());
        ui->spinBox_ypos->setValue(selectedItems.front()->pos().y());

    } else{
        ui->stackedWidget->setCurrentIndex(2);

    }
}

void MainWindow::handleUpdatePos()
{
    if(selectedItems.count() == 1){
        ui->spinBox_rotation->setValue(selectedItems.front()->rotation());
        ui->spinBox_scale->setValue(selectedItems.front()->transform().m11());
        ui->spinBox_xpos->setValue(selectedItems.front()->pos().x());
        ui->spinBox_ypos->setValue(selectedItems.front()->pos().y());
    }
}

void MainWindow::handleChangeXPos(int value)
{
    if(ui->spinBox_xpos->hasFocus()){
        selectedItems.front()->setPos(value,selectedItems.front()->pos().y());
    }
}

void MainWindow::handleChangeYPos(int value)
{
    if(ui->spinBox_ypos->hasFocus()){
        selectedItems.front()->setPos(selectedItems.front()->pos().x(),value);
    }
}

void MainWindow::handleChangeScale(double value)
{
    if(ui->spinBox_scale->hasFocus()){
        QTransform trans;
        trans.scale(value,value);
        selectedItems.front()->setTransform(trans);
    }
}

void MainWindow::handleChangeRotation(double value)
{
    if(ui->spinBox_rotation->hasFocus()){
        selectedItems.front()->setRotation(value);
    }
}

void MainWindow::handleUpdateImportOptions()
{
    QList<ItemTemplate*> tempsToChange;
    foreach(QModelIndex index, ui->element_tree->elementView->selectionModel()->selectedIndexes()){
        ItemElement* item = static_cast<ItemElement*>(elementModel->itemFromIndex(index));
        tempsToChange.append(item->getTemplate());
    }

    ColliderDialog *colDiag = new ColliderDialog(tempsToChange.front(),this);

    colDiag->setModal(true);
    colDiag->exec();

    delete colDiag;

    // ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->viewport()->update();

}

void MainWindow::handleLevelChange(QItemSelection seleceted, QItemSelection deselected)
{
    QModelIndexList selectedIndexes = ui->treeView_level->selectionModel()->selectedIndexes();
    bool showColliders = false;
    bool rotateEnabled = false;
    bool scaleEnabled = false;
    if(selectedLevel){
        selectedLevel->clearSelection();
        showColliders = selectedLevel->showColliders();
        rotateEnabled = selectedLevel->rotate();
        scaleEnabled = selectedLevel->scale();

        disconnect(selectedLevel, SIGNAL(onRequestTemplates(QPointF)),
                   ui->dockWidgetContents,SLOT(handleRequestedTemplates(QPointF)));

//        disconnect(selectedLevel, SIGNAL(selectionChanged()),
//                   this,SLOT(handleSceneSelectionChanged()));

        disconnect(selectedLevel, SIGNAL(onUpdatePos()),
                   this, SLOT(handleUpdatePos()));

//        disconnect (selectedLevel, SIGNAL(onItemDeleted(QList<QGraphicsItem*>)),
//                    this, SLOT(handleOnItemsDeleted(QList<QGraphicsItem*>)));
        ui->element_tree->disconnectScene();

    }

    if(selectedIndexes.count() > 0){

        selectedLevel = static_cast<MainScene*>(levelModel->itemFromIndex(selectedIndexes.front()));
        ui->element_tree->setScene(selectedLevel);

        selectedLevel->setShowColliders(showColliders);
        selectedLevel->setRotate(rotateEnabled);
        selectedLevel->setScale(scaleEnabled);
        elementModel->layoutAboutToBeChanged();
        elementModel->setRoot(selectedLevel->getRoot());
        elementModel->layoutChanged();
        ui->graphicsView->setScene(selectedLevel);

        connect(selectedLevel, SIGNAL(onRequestTemplates(QPointF)),
                ui->dockWidgetContents,SLOT(handleRequestedTemplates(QPointF)));

//        connect(selectedLevel, SIGNAL(selectionChanged()),
//                this,SLOT(handleSceneSelectionChanged()));

        connect(selectedLevel, SIGNAL(onUpdatePos()),
                this, SLOT(handleUpdatePos()));

//        connect (selectedLevel, SIGNAL(onItemDeleted(QList<QGraphicsItem*>)),
//                 this, SLOT(handleOnItemsDeleted(QList<QGraphicsItem*>)));

        ui->element_tree->connectToScene();
    }
}

void MainWindow::handleAddLevel()
{
    MainScene *newLevel = new MainScene();
    levelModel->insertItem(QModelIndex(), newLevel);
    ui->treeView_level->selectionModel()->clearSelection();
    ui->treeView_level->selectionModel()->setCurrentIndex(levelModel->indexFromItem(newLevel), QItemSelectionModel::Select);
}

void MainWindow::handleRemoveLevel()
{

}

void MainWindow::handleShowCollider(bool active)
{   qDebug() << "iam in";
    if(selectedLevel){
        selectedLevel->setShowColliders(active);
        ui->graphicsView->viewport()->update();
    }
    qDebug() << "iam out";
}

void MainWindow::handleScaleToggled(bool value)
{
    if(selectedLevel){
        selectedLevel->setScale(value);
        ui->graphicsView->viewport()->update();
    }
}

void MainWindow::handleRotateToggled(bool value)
{
    if(selectedLevel){
        selectedLevel->setRotate(value);
        ui->graphicsView->viewport()->update();
    }
}

void MainWindow::loadLayouts()
{
    foreach(QAction* a, layoutActions) {
        ui->menuView->removeAction(a);
    }

    layouts = layoutWidget->getLayouts();
    layoutActions.clear();

    for(int i=0; i<layouts.count(); ++i){
        layoutActions.append(ui->menuView->addAction(
                                 layouts[i].first));
        connect(layoutActions.last(), SIGNAL(triggered()), this, SLOT(handleRestoreLayout()));
    }
}


