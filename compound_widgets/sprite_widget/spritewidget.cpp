#include "spritewidget.h"
#include "ui_spritewidget.h"

#include <QGraphicsDropShadowEffect>

const QString Err = "<font color=\"#EE9B0F\">the specified %1 property does not match the image propertions</font><br />";
const QString GreetingChoose = "<font color=\"#01BB29\">Choose a sprite sheet to be loaded</font>";
const QString GreetingProperties = "<font color=\"#01BB29\">Set properties for your sprite sheet</font>";

SpriteWidget::SpriteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpriteWidget)
{
    ui->setupUi(this);

    // create scenes
    mSheetScene = new QGraphicsScene();
    mAnimationScene = new QGraphicsScene();

    // setup the two views
    ui->sheetview->setScene(mSheetScene);
    ui->animationview->setScene(mAnimationScene);

    ui->label_info->setText(GreetingChoose);
    ui->label_info->setWordWrap(true);

    // create the list model
    model = new AnimationTreeItemModel(6);
    ui->treeView->setModel(model);

    // set the spinbox delegate for row/column elements
    rowDelegate = new SpinBoxDelegate();
    columnDelegate = new SpinBoxDelegate();
    speedDelegate = new SpinBoxDelegate();

    ui->treeView->setItemDelegateForColumn(1, rowDelegate);
    ui->treeView->setItemDelegateForColumn(2, columnDelegate);
    ui->treeView->setItemDelegateForColumn(3, rowDelegate);
    ui->treeView->setItemDelegateForColumn(4, columnDelegate);
    speedDelegate->setValueRange(1,10000);
    ui->treeView->setItemDelegateForColumn(5, speedDelegate);

    // disable contextual interface
    ui->button_add_animation->setEnabled(false);
    ui->button_remove_animation->setEnabled(false);
    ui->groupBox->setEnabled(false);

    //create connections
    //setupConnections();
}

void SpriteWidget::setupConnections()
{
    connect(ui->button_add_animation, SIGNAL(clicked()),
            this, SLOT(handleAddAnimationSequence()));
    // connect the apply button to the animated sprite
    connect(ui->button_apply_properties, SIGNAL(clicked()),
            this, SLOT(handleSetSpriteProperties()));
    // get selection from treeview
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(handleAnimationSelectionChanged(QModelIndex)));

    connect(ui->button_remove_animation, SIGNAL(clicked()),
            this, SLOT(handleRemoveAnimationClick()));

    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(handleDataChanged(QModelIndex,QModelIndex)));

    connect(ui->row_edit, SIGNAL(valueChanged(int)),
            this, SLOT(handleRowCountChanged(int)));

    connect(ui->column_edit, SIGNAL(valueChanged(int)),
            this, SLOT(handleColumnCountChanged(int)));

    connect(ui->height_edit, SIGNAL(valueChanged(int)),
            this, SLOT(handleSpriteHeightChanged(int)));

    connect(ui->width_edit, SIGNAL(valueChanged(int)),
            this, SLOT(handleSpriteWidthChanged(int)));
}

void SpriteWidget::onLoadSelectedItem(ItemTemplate *item)
{
    ui->button_add_animation->setEnabled(false);
    ui->button_remove_animation->setEnabled(false);
    mTimer.stop();
    mSheetScene->clear();
    mAnimationScene->clear();

    mCurrentItem = item;
    if(!mCurrentItem->getAnimation()){
        mCurrentItem->setAnimation(new Animation());
    }

    if(mCurrentItem->getAnimation()->getSprite()) {
        mSprite = new AnimatedSprite(*mCurrentItem->getAnimation()->getSprite());
        connect(&mTimer, SIGNAL(timeout()),
                mSprite, SLOT(handleTimeStep()));
        mAnimationScene->addItem(mSprite);

        mTimer.start();
        ui->button_add_animation->setEnabled(true);
        ui->button_remove_animation->setEnabled(true);
    }

    mImage = QImage(mCurrentItem->image());

    // add a grid to the view
    mGrid = new GraphicsGridItem(QRectF(0,0,mImage.width(), mImage.height()));
    mGrid->setGrid(mCurrentItem->getAnimation()->getRows(),
                   mCurrentItem->getAnimation()->getColumns(),
                   mCurrentItem->getAnimation()->getWidth(),
                   mCurrentItem->getAnimation()->getHeight());

    ui->row_edit->setValue(mCurrentItem->getAnimation()->getRows());
    ui->column_edit->setValue(mCurrentItem->getAnimation()->getColumns());
    ui->height_edit->setValue(mCurrentItem->getAnimation()->getHeight());
    ui->width_edit->setValue(mCurrentItem->getAnimation()->getWidth());



    // set the root index of the item model
    model->setRoot(mCurrentItem->getAnimation());
    model->layoutChanged();

    mSheetScene->setSceneRect(mImage.rect());
    ui->sheetview->setMaximumSize(QSize(mImage.width()+25, mImage.height()+25));

    mSheetScene->addItem(mGrid);

    ui->groupBox->setEnabled(true);
    ui->label_info->setText(GreetingProperties);

    onFileSelected();
}

SpriteWidget::~SpriteWidget()
{
    delete mAnimationScene;
    delete mSheetScene;
    delete rowDelegate;
    delete columnDelegate;
    delete speedDelegate;
    delete model;
    delete ui;
}

void SpriteWidget::handleRemoveAnimationClick()
{
    QModelIndexList index = ui->treeView->selectionModel()->selectedRows();

    if(!index.empty()) {
        AbstractTreeItem* ani = static_cast<AbstractTreeItem *>(model->itemFromIndex(index[0]));

        if(ani) {
            model->getRoot()->removeChild(ani);
            model->layoutChanged();
        }
        mSprite->setVisible(false);
    }
}

void SpriteWidget::handleDataChanged(QModelIndex idx1,QModelIndex idx2)
{
    Q_UNUSED(idx2);

    AnimationsSequence* ani = static_cast<AnimationsSequence *>(model->itemFromIndex(idx1));

    if(ani) {
        updateAnimationScene(ani);
    }
}

void SpriteWidget::handleAddAnimationSequence()
{
    AnimationsSequence *itm = new AnimationsSequence(
                "animation"+QString::number(model->getRoot()->childCount()),
                1, 1, ui->row_edit->value(), ui->column_edit->value(), 150, model->getRoot());
    model->getRoot()->addChild(itm);
    model->layoutChanged();
    mSprite->setVisible(true);
}

void SpriteWidget::handleSetSpriteProperties()
{
    //    if(!mFile.isEmpty()) {
    QString out;

    if(mImage.height() < ui->height_edit->value()) {
        out.append(QString(Err).arg("height"));
    }

    if(mImage.width() < ui->width_edit->value()) {
        out.append(QString(Err).arg("width"));
    }

    if(mImage.width() < (ui->width_edit->value() * ui->column_edit->value())) {
        out.append(QString(Err).arg("column"));
    }

    if(mImage.height() < (ui->height_edit->value() * ui->row_edit->value())) {
        out.append(QString(Err).arg("row"));
    }

    // TODO: fix this bad logic
    if(out.isEmpty()) {
        out = "<font color=\"#01BB29\">Settings applied</font>";
        setAnimationScene();
        //set the pixmap of the template so it shows a single cell of the spritesheet
        mCurrentItem->setPixmap(new QPixmap(mCurrentItem->getAnimation()->getSpriteSheet().copy(0,0,ui->width_edit->value(), ui->height_edit->value())));
        mCurrentItem->setIcon(*mCurrentItem->pixmap());

        mCurrentItem->getPixmapItem()->setPixmap(*mCurrentItem->pixmap());
        QImage img = mCurrentItem->pixmap()->toImage();
         mCurrentItem->setContour(mImgProc.findContoursFromQImage(img));
         mCurrentItem->calculateSceneRect();

    }
    ui->label_info->setText(out);
}

void SpriteWidget::handleAnimationSelectionChanged(QModelIndex index)
{
    AnimationsSequence* ani = static_cast<AnimationsSequence *>(model->itemFromIndex(index));

    if(ani) {
        updateAnimationScene(ani);
    }
}

void SpriteWidget::handleRowCountChanged(int value)
{
    mGrid->setRows(value);
    mCurrentItem->getAnimation()->setRows(value);
}

void SpriteWidget::handleColumnCountChanged(int value)
{
    mGrid->setColumns(value);
    mCurrentItem->getAnimation()->setColumns(value);
}

void SpriteWidget::handleSpriteWidthChanged(int value)
{
    mGrid->setVSpacing(value);
    mCurrentItem->getAnimation()->setWidth(value);
}

void SpriteWidget::handleSpriteHeightChanged(int value)
{
    mGrid->setHSpacing(value);
    mCurrentItem->getAnimation()->setHeight(value);
}

void SpriteWidget::updateAnimationScene(AnimationsSequence *item)
{
    mSprite->toggleAnimate(true);
    mSprite->updateAnimation(item);
    mTimer.setInterval(item->getAnimationSpeed());
    mAnimationScene->setSceneRect(0,0,
                                  ui->width_edit->value(),ui->height_edit->value());
    mSprite->setVisible(true);
}

void SpriteWidget::setAnimationScene()
{
    mAnimationScene->clear();

    rowDelegate->setValueRange(1, ui->row_edit->value());
    columnDelegate->setValueRange(1, ui->column_edit->value());

    mCurrentItem->getAnimation()->setSprite(new AnimatedSprite(mCurrentItem->path()));

    mCurrentItem->getAnimation()->getSprite()->setRowColumnCount(ui->row_edit->value(), ui->column_edit->value());
    mCurrentItem->getAnimation()->getSprite()->setAnimationProperties(ui->width_edit->value(), ui->height_edit->value());
    mSprite = new AnimatedSprite(*mCurrentItem->getAnimation()->getSprite());
    mSprite->setVisible(false);
    // setup a timed update
    connect(&mTimer, SIGNAL(timeout()),
            mSprite, SLOT(handleTimeStep()));

    mAnimationScene->addItem(mSprite);

    mTimer.setInterval(150);
    mTimer.start();

    ui->button_add_animation->setEnabled(true);
    ui->button_remove_animation->setEnabled(true);
}

void SpriteWidget::onFileSelected()
{
    Animation *ani = static_cast<Animation* >(model->getRoot());
    ani->setSpriteSheet(QPixmap::fromImage(mImage));
    mSheetScene->addPixmap(QPixmap::fromImage(mImage));
}



