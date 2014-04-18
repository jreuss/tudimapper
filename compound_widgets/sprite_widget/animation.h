#ifndef ANIMATION_H
#define ANIMATION_H

#include "stable_pch.h"
#include "animatedsprite.h"
#include "items/abstracttreeitem.h"

class Animation : public AbstractTreeItem
{
public:
    Animation();

    QSize getAnimationSize() const;
    void setAnimationSize(const QSize &value);

    void setWidth(int width);
    int getWidth() const;

    void setHeight(int height);
    int getHeight() const;

    QPixmap getSpriteSheet() const;
    void setSpriteSheet(const QPixmap &value);

    int getRows() const;
    void setRows(int value);

    int getColumns() const;
    void setColumns(int value);

    AnimatedSprite *getSprite() const;
    void setSprite(AnimatedSprite *value);

private:
    QSize mAnimationSize;
    int mRows, mColumns;
    AnimatedSprite *mSprite;
    QPixmap mSpriteSheet;
};

#endif // ANIMATION_H
