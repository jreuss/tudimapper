#ifndef SPRITEWIDGET_H
#define SPRITEWIDGET_H

#include "stable_pch.h"
#include "animatedsprite.h"
#include "animation.h"
#include "animationssequence.h"
#include "animationtreeitemmodel.h"
#include "delegates/spinboxdelegate.h"
#include "items/itemtemplate.h"
#include "graphicsgriditem.h"
#include "utility/imgproc.h"

namespace Ui {
class SpriteWidget;
}

class SpriteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpriteWidget(QWidget *parent = 0);
    void setupConnections();
    void onLoadSelectedItem(ItemTemplate *item);
    ~SpriteWidget();
public slots:
    void handleRemoveAnimationClick();
    void handleDataChanged(QModelIndex,QModelIndex);
    void handleAddAnimationSequence();
    void handleSetSpriteProperties();
    void handleAnimationSelectionChanged(QModelIndex);
    void handleRowCountChanged(int);
    void handleColumnCountChanged(int);
    void handleSpriteWidthChanged(int);
    void handleSpriteHeightChanged(int);

private:
    void updateAnimationScene(AnimationsSequence *item);
    void setAnimationScene();

    Ui::SpriteWidget *ui;
    QGraphicsScene *mSheetScene, *mAnimationScene;
    AnimatedSprite *mSprite;
    QTimer mTimer;
    QString mFile;
    QImage mImage;
    AnimationTreeItemModel *model;
    GraphicsGridItem *mGrid;
    SpinBoxDelegate *rowDelegate, *columnDelegate, *speedDelegate;
    ItemTemplate * mCurrentItem;
    int mWidth, mHeight;
    void onFileSelected();
    //We need to have one improg maybe in the colliderscene
    ImgProc mImgProc;
};

#endif // SPRITEWIDGET_H
