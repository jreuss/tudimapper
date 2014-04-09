#include "animation.h"

Animation::Animation() : AbstractTreeItem(0)
{
    mAnimationSize = QSize(0,0);
    mSprite = NULL;
    mRows = 1;
    mColumns = 1;
}

QSize Animation::getAnimationSize() const
{
    return mAnimationSize;
}

void Animation::setAnimationSize(const QSize &value)
{
    mAnimationSize = value;
}

void Animation::setWidth(int width)
{
    mAnimationSize.setWidth(width);
}

int Animation::getWidth() const
{
    return mAnimationSize.width();
}

void Animation::setHeight(int height)
{
    mAnimationSize.setHeight(height);
}

int Animation::getHeight() const
{
    return mAnimationSize.height();
}

QPixmap Animation::getSpriteSheet() const
{
    return mSpriteSheet;
}

void Animation::setSpriteSheet(const QPixmap &value)
{
    mSpriteSheet = value;
}
int Animation::getRows() const
{
    return mRows;
}

void Animation::setRows(int value)
{
    mRows = value;
}
int Animation::getColumns() const
{
    return mColumns;
}

void Animation::setColumns(int value)
{
    mColumns = value;
}

AnimatedSprite *Animation::getSprite() const
{
    return mSprite;
}

void Animation::setSprite(AnimatedSprite *value)
{
    mSprite = value;
}





