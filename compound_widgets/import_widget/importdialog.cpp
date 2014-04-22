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
    ui->stack_pages->setCurrentIndex(0);
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
            ui->toolBtn_collider->setChecked(true);
            handleToggleAnimationButton(true);
        }  else if(type == ItemTemplate::Split) {
            ui->toolBtn_animation->setChecked(false);
            ui->toolBtn_split->setChecked(true);
            ui->toolBtn_collider->setChecked(false);
            handleToggleSplitButton(true);
        }

        // animate the pages when switching

    }

}

void ImportDialog::handleApplySplitOption()
{
    if(ui->radio_split->isChecked())
    {

    }

    if(ui->radio_keepAsOne->isChecked()){
        mCurrentItm->setItemType(ItemTemplate::Single);
        mModel->layoutChanged();
        toogleToolButtons();
        ui->toolBtn_collider->setChecked(true);
    }

    if(ui->radio_spritesheet->isChecked())
    {
        mCurrentItm->setItemType(ItemTemplate::SpriteSheet);
        mModel->layoutChanged();
        toogleToolButtons();
        ui->toolBtn_animation->setChecked(true);
    }

    if(ui->radio_split->isChecked())
    {
        //QImage templates = mImgProc.createMatchImage(mCurrentItm->contour(),mCurrentItm->path(), 0.18);
        //mScene->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(templates)));

        mCurrentItm->setItemType(ItemTemplate::Split);
        mModel->layoutChanged();
        toogleToolButtons();
        ui->toolBtn_split->setChecked(true);

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

void ImportDialog::handleToggleTreelistEnabled()
{
    ui->treeView->setEnabled (!ui->treeView->isEnabled ());
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
            this,SLOT(handleApplySplitOption()));

    connect(ui->btn_import, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(ui->toolBtn_collider,SIGNAL(toggled(bool)),
            this,SLOT(handleToggleColliderButton(bool)));

    connect(ui->toolBtn_animation,SIGNAL(toggled(bool)),
            this,SLOT(handleToggleAnimationButton(bool)));
    connect(ui->toolBtn_split,SIGNAL(toggled(bool)),
            this,SLOT(handleToggleSplitButton(bool)));
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
    emit onImportAccept(static_cast<ItemTemplate*>(mModel->getRoot()));
    QDialog::accept();
}
