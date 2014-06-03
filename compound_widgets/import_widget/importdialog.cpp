#include "importdialog.h"
#include "ui_importdialog.h"



ImportDialog::ImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    mCurrentItm = NULL;
    mModel = new ImportTreeModel();
    ui->treeView->setModel (mModel);
    setupConnections();
    mScene = new QGraphicsScene();
    ui->graphics_groupopt->setScene(mScene);
    toogleToolButtons();
    ui->stack_pages->setCurrentIndex(0);
}

ImportDialog::ImportDialog(QList<QUrl> imgFiles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    mCurrentItm = NULL;
    ui->spritewidget->setupConnections ();
    ui->colliderwidget->setupConnections();
    ui->splitwidget->setupConnections();
    mModel = new ImportTreeModel();
    ui->treeView->setModel (mModel);

    setupConnections();
    mScene = new QGraphicsScene();
    ui->graphics_groupopt->setScene(mScene);
    mModel->addItemsFromUrls (imgFiles);
    toogleToolButtons();
    //this->setWindowState(Qt::WindowMaximized);

    //ui->stack_pages->setCurrentIndex(0);
}

ImportDialog::ImportDialog(QList<ItemTemplate*> tempsToChange, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    mCurrentItm = NULL;
    ui->spritewidget->setupConnections ();
    ui->colliderwidget->setupConnections();
    ui->splitwidget->setupConnections();
    mModel = new ImportTreeModel();
    ui->treeView->setModel (mModel);

    setupConnections();
    mScene = new QGraphicsScene();
    ui->graphics_groupopt->setScene(mScene);
    foreach(ItemTemplate* temp, tempsToChange){
        mModel->insertItem(mModel->getRoot()->getChildren().count(), mModel->getRoot(), temp);
    }

    toogleToolButtons();
   // ui->stack_pages->setCurrentIndex(0);
}

ImportDialog::~ImportDialog()
{
    delete mScene;
    delete mModel;
    delete ui;
}

void ImportDialog::handleImportItemSelectionChanged(QModelIndex index)
{
    if(index != mCurrentIndex)
    {

        mCurrentIndex = index;

        mCurrentItm = static_cast<ItemTemplate* >(mModel->itemFromIndex (index));

        int type = mCurrentItm->importType ();


        toogleToolButtons();

        if(type == ItemTemplate::Single){
            ui->toolBtn_animation->setChecked(false);
            ui->toolBtn_split->setChecked(false);
            ui->toolBtn_collider->setChecked(true);
            handleToggleColliderButton(true);
        } else if (type == ItemTemplate::Group){
            mScene->clear();
            mScene->setSceneRect(mCurrentItm->image().rect());
            QGraphicsPixmapItem *pmap = new QGraphicsPixmapItem(QPixmap::fromImage(
                                                                    mCurrentItm->image()));
            mScene->addItem(pmap);

            ui->stack_pages->setCurrentIndex(type);

            animatePage ();


        } else if(type == ItemTemplate::SpriteSheet) {
            ui->toolBtn_animation->setChecked(true);
            ui->toolBtn_split->setChecked(false);
            ui->toolBtn_collider->setChecked(false);
            handleToggleAnimationButton(true);
        }  else if(type == ItemTemplate::Split) {
            ui->toolBtn_animation->setChecked(false);
            ui->toolBtn_split->setChecked(true);
            ui->toolBtn_collider->setChecked(false);
            handleToggleSplitButton(true);
        }
    }

}

void ImportDialog::handleApplyGroupOption()
{
    if(ui->radio_split->isChecked())
    {

    }

    if(ui->radio_keepAsOne->isChecked()){
        mCurrentItm->setItemType(ItemTemplate::Single);
        mModel->layoutChanged();
        toogleToolButtons();
        ui->toolBtn_collider->setChecked(true);
        handleToggleColliderButton(true);
    }

    if(ui->radio_spritesheet->isChecked())
    {
        mCurrentItm->setItemType(ItemTemplate::SpriteSheet);
        mModel->layoutChanged();
        toogleToolButtons();
        ui->toolBtn_animation->setChecked(true);
        handleToggleAnimationButton(true);
    }

    if(ui->radio_split->isChecked())
    {

        mCurrentItm->setItemType(ItemTemplate::Split);
        mModel->layoutChanged();
        toogleToolButtons();
        ui->toolBtn_split->setChecked(true);
        handleToggleSplitButton(true);

    }
}


void ImportDialog::handleImportItemNameChanged(QString text)
{
    QModelIndex index = ui->treeView->currentIndex ();
    if(!text.isEmpty ()) {
        mModel->setData (index,text,Qt::EditRole);
    } else {
        mModel->setData (index,QString("Item"),Qt::EditRole);
    }
}

void ImportDialog::handleToggleMainUIEnabled()
{
    ui->treeView->setEnabled (!ui->treeView->isEnabled ());
    ui->btn_import->setEnabled(!ui->btn_import->isEnabled ());
    ui->btn_cancel->setEnabled(!ui->btn_cancel->isEnabled ());
}

void ImportDialog::handleToggleAnimationButton(bool animationEnabled)
{
    if(animationEnabled)
    {
        ui->stack_pages->setCurrentIndex(3);
        ui->spritewidget->onLoadSelectedItem (mCurrentItm);
        animatePage();
    }
}

void ImportDialog::handleToggleSplitButton(bool splitEnabled)
{
    if(splitEnabled)
    {
        ui->stack_pages->setCurrentIndex(4);
        ui->splitwidget->onLoadSelectedItem (mCurrentItm);
        animatePage();
    }
}

void ImportDialog::handleRemoveSplitParent()
{
    ItemTemplate* itemToRemove = mCurrentItm;
    ui->stack_pages->setCurrentIndex(0);
    animatePage();
    mCurrentItm = NULL;
    toogleToolButtons();
    mModel->removeItem(itemToRemove);
}

void ImportDialog::handleExpandSplit(ItemTemplate *item)
{
    ui->treeView->expand(mModel->indexFromItem(item));
}

void ImportDialog::handleSelectFirstUrl(ItemTemplate *item)
{
    ui->treeView->selectionModel()->clearSelection();
    ui->treeView->selectionModel()->setCurrentIndex(mModel->indexFromItem(item), QItemSelectionModel::Select);
    handleImportItemSelectionChanged(mModel->indexFromItem(item));
}

void ImportDialog::handleSplitSceneDoubleClick(ItemTemplate *item)
{
    ui->treeView->selectionModel()->clearSelection();
    ui->treeView->selectionModel()->setCurrentIndex(mModel->indexFromItem(item), QItemSelectionModel::Select);
    handleImportItemSelectionChanged(mModel->indexFromItem(item));
}

void ImportDialog::handleToggleColliderButton(bool colliderEnabled)
{
    if(colliderEnabled)
    {
        ui->stack_pages->setCurrentIndex(1);
        ui->colliderwidget->onLoadSelectedItem (mCurrentItm);
        animatePage();
    }
}


void ImportDialog::setupConnections()
{
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(handleImportItemSelectionChanged(QModelIndex)));
    connect(ui->splitwidget,SIGNAL(onSplitAplied(ItemTemplate*,bool,float))
            ,mModel,SLOT(handleSplit(ItemTemplate*, bool,float)));

    connect(ui->splitwidget,SIGNAL(onSplitAndAddToSceneAplied(ItemTemplate*,bool,float))
            ,mModel,SLOT(handleSplitAndAddToScene(ItemTemplate*,bool,float)));

    connect(ui->btn_apply,SIGNAL(clicked()),
            this,SLOT(handleApplyGroupOption()));

    connect(ui->btn_import, SIGNAL(clicked()),
            this, SLOT(accept()));

    //we need to overide reject so it delet all pointers
    connect(ui->btn_cancel, SIGNAL(clicked()),
            this, SLOT(reject()));

    connect(ui->toolBtn_collider,SIGNAL(clicked(bool)),
            this,SLOT(handleToggleColliderButton(bool)));

    connect(ui->toolBtn_animation,SIGNAL(clicked(bool)),
            this,SLOT(handleToggleAnimationButton(bool)));

    connect(ui->toolBtn_split,SIGNAL(clicked(bool)),
            this,SLOT(handleToggleSplitButton(bool)));

    connect(mModel,SIGNAL(onRemoveSplitParent()),
            this,SLOT(handleRemoveSplitParent()));

    connect(mModel,SIGNAL(onExpandSplit(ItemTemplate*)),
            this,SLOT(handleExpandSplit(ItemTemplate*)));

    connect(mModel,SIGNAL(onSelectFirstUrl(ItemTemplate*)),
            this,SLOT(handleSelectFirstUrl(ItemTemplate*)));

    connect(ui->splitwidget,SIGNAL(onDoubleClick(ItemTemplate*)),
            this,SLOT(handleSplitSceneDoubleClick(ItemTemplate*)));



}

void ImportDialog::animatePage()
{
    QWidget *page = ui->stack_pages->currentWidget();
    QRect origin = page->rect();
    QRect offset = QRect(page->pos().x() - page->width(), page->pos().y(),
                         page->width(), page->height());

    QPropertyAnimation *anim = new QPropertyAnimation(page, "geometry");
    anim->setDuration(200);
    anim->setStartValue(offset);
    anim->setEndValue(origin);
    anim->setEasingCurve(QEasingCurve::OutQuad);

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void ImportDialog::toogleToolButtons()
{   if(mCurrentItm){
        if(mCurrentItm->importType() == ItemTemplate::Single){
            ui->toolBtn_animation->setEnabled(false);
            ui->toolBtn_collider->setEnabled(true);
            ui->toolBtn_split->setEnabled(false);
        } else if(mCurrentItm->importType() == ItemTemplate::SpriteSheet)
        {
            ui->toolBtn_animation->setEnabled(true);
            ui->toolBtn_collider->setEnabled(true);
            ui->toolBtn_split->setEnabled(false);
        }else if(mCurrentItm->importType() == ItemTemplate::Split)
        {
            ui->toolBtn_animation->setEnabled(false);
            ui->toolBtn_collider->setEnabled(false);
            ui->toolBtn_split->setEnabled(true);

        } else {
            ui->toolBtn_animation->setEnabled(false);
            ui->toolBtn_collider->setEnabled(false);
            ui->toolBtn_split->setEnabled(false);
        }
    } else {
        ui->toolBtn_animation->setEnabled(false);
        ui->toolBtn_collider->setEnabled(false);
        ui->toolBtn_split->setEnabled(false);
    }
}

void ImportDialog::accept()
{
    bool canAccept = true;
    ItemTemplate* currentItem;
    QModelIndex rootIndex = mModel->indexFromItem(mModel->getRoot());
    for(int i=0;i<mModel->rowCount(rootIndex);i++){
        QModelIndex index = mModel->index(i,0,rootIndex);
        currentItem = static_cast<ItemTemplate*>(mModel->itemFromIndex(index));
        if(currentItem->importType() == ItemTemplate::Group)
            canAccept = false;
    }
    if(canAccept){
        emit onImportAccept(static_cast<ItemTemplate*>(mModel->getRoot()));
        QDialog::accept();
    }
    else{
        QMessageBox::warning(
            this,
            tr("Application Name"),
            tr("You need to set propeties for all the groups(marked with red)") );
    }
}
