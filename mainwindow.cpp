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
    ui->treeView_elements->setModel(elementModel);
    ui->treeView_elements->setSelectionMode(QAbstractItemView::ExtendedSelection);
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
    connect(ui->mainToolbar->alignX, SIGNAL(clicked()),
            this, SLOT(handleAlignItemsX()));

    connect(ui->mainToolbar->alignY, SIGNAL(clicked()),
            this, SLOT(handleAlignItemsY()));

    connect(ui->graphicsView, SIGNAL(zoomChanged(double)),
            (ui->mainToolbar), SLOT(handleZoomChanged(double)));

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

    connect (ui->treeView_elements, SIGNAL(clicked(QModelIndex)),
             this, SLOT(handleTreeviewSelectionChanged(QModelIndex)));

    connect (ui->treeView_elements->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
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

void MainWindow::handleTemplatesRecieved(QPointF pos, QList<ItemTemplate *> templates)
{
    foreach(ItemTemplate* item, templates){
        if(item->importType() == ItemTemplate::Split){
            QList<QGraphicsItem*> elements = item->getSplitScene()->items();
            foreach(QGraphicsItem* currentItem, elements){
                ItemElement *element = new ItemElement(static_cast<ItemElement*>(currentItem));
                element->setParentItem(selectedLevel->getRoot());
                elementModel->insertItem(elementModel->getRoot()->getChildren().count(),elementModel->getRoot(),element);

                element->setPos(currentItem->pos());
                element->setScale(currentItem->scale());
            }

        } else {
            ItemElement *element = new ItemElement(item);
            element->setParentItem(selectedLevel->getRoot());
            elementModel->insertItem(elementModel->getRoot()->getChildren().count(),elementModel->getRoot(),element);

            element->setPos(pos);
        }
    }
}

void MainWindow::handleAddNewLayout()
{
    layoutWidget->show();
}

void MainWindow::handleUpdateLayoutMenu()
{
    qDebug() << "called";
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
{   ui->treeView_elements->selectionModel()->clearSelection();
    foreach(QGraphicsItem *it, selectedLevel->selectedItems())
    {
        AbstractTreePixmapItem *itm = static_cast<AbstractTreePixmapItem *>(it);
        QModelIndex index = elementModel->indexFromItem(itm);
        ui->treeView_elements->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void MainWindow::handleTreeviewSelectionChanged(QModelIndex pressedOn)
{
    Q_UNUSED(pressedOn);

    QModelIndexList idxList = ui->treeView_elements->selectionModel()->selectedIndexes();
    foreach(QGraphicsItem *itm, selectedLevel->selectedItems ()){
        itm->setSelected (false);
    }
    foreach (QModelIndex idx , idxList) {
        elementModel->itemFromIndex(idx)->setSelected(true);
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
    QModelIndexList selectedIndexes = ui->treeView_elements->selectionModel()->selectedIndexes();
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
    foreach(QModelIndex index, ui->treeView_elements->selectionModel()->selectedIndexes()){
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

        disconnect(selectedLevel, SIGNAL(selectionChanged()),
                   this,SLOT(handleSceneSelectionChanged()));

        disconnect(selectedLevel, SIGNAL(onUpdatePos()),
                   this, SLOT(handleUpdatePos()));

        disconnect (selectedLevel, SIGNAL(onItemDeleted(QList<QGraphicsItem*>)),
                    this, SLOT(handleOnItemsDeleted(QList<QGraphicsItem*>)));

    }

    if(selectedIndexes.count() > 0){

        selectedLevel = static_cast<MainScene*>(levelModel->itemFromIndex(selectedIndexes.front()));
        selectedLevel->setShowColliders(showColliders);
        selectedLevel->setRotate(rotateEnabled);
        selectedLevel->setScale(scaleEnabled);
        elementModel->layoutAboutToBeChanged();
        elementModel->setRoot(selectedLevel->getRoot());
        elementModel->layoutChanged();
        ui->graphicsView->setScene(selectedLevel);

        connect(selectedLevel, SIGNAL(onRequestTemplates(QPointF)),
                ui->dockWidgetContents,SLOT(handleRequestedTemplates(QPointF)));

        connect(selectedLevel, SIGNAL(selectionChanged()),
                this,SLOT(handleSceneSelectionChanged()));

        connect(selectedLevel, SIGNAL(onUpdatePos()),
                this, SLOT(handleUpdatePos()));

        connect (selectedLevel, SIGNAL(onItemDeleted(QList<QGraphicsItem*>)),
                 this, SLOT(handleOnItemsDeleted(QList<QGraphicsItem*>)));
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
{
    if(selectedLevel){
        selectedLevel->setShowColliders(active);
        ui->graphicsView->viewport()->update();
    }
}

void MainWindow::handleScaleToggled(bool value)
{
    if(selectedLevel){
        selectedLevel->setScale(value);
        ui->graphicsView->viewport()->update();
    }
}

void MainWindow::handleAlignItemsY()
{
    if(selectedLevel) {
        if (selectedItems.count() != 0) {

            int y = 1000000;
            for (int i=0; i < selectedItems.count(); ++i) {
                if(selectedItems[i]->pos().y() < y) {
                    y = selectedItems[i]->pos().y();
                }
            }

            for (int i=0; i < selectedItems.count(); ++i) {
                selectedItems[i]->setPos(selectedItems[i]->pos().x(), y);
            }
        }
    }
}

void MainWindow::handleAlignItemsX()
{
    if(selectedLevel) {
        if (selectedItems.count() != 0) {

            int x = 1000000;
            for (int i=0; i < selectedItems.count(); ++i) {
                if(selectedItems[i]->pos().x() < x) {
                    x = selectedItems[i]->pos().x();
                }
            }

            for (int i=0; i < selectedItems.count(); ++i) {
                selectedItems[i]->setPos(x, selectedItems[i]->pos().y());
            }
        }
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


