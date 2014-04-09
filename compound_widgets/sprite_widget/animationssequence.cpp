#include "animationssequence.h"

AnimationsSequence::AnimationsSequence(AbstractTreeItem *parent)
    : AbstractTreeItem(parent)
{
    mName = "";
    mAnimationSpeed = 150;
    mStartRow = 0;
    mStartCol = 0;
    mEndRow = 0;
    mEndCol = 0;
}

AnimationsSequence::AnimationsSequence(const QString &name, int startRow, int startCol, int endRow, int endCol, int animationSpeed, AbstractTreeItem *parent) :
    AbstractTreeItem(parent)
{
    mName = name;
    mStartRow = startRow;
    mStartCol = startCol;
    mEndRow = endRow;
    mEndCol = endCol;
    mAnimationSpeed = animationSpeed;
}

int AnimationsSequence::getStartColumn() const
{
    return mStartCol;
}

void AnimationsSequence::setStartColumn(int value)
{
    mStartCol = value;
}
int AnimationsSequence::getStartRow() const
{
    return mStartRow;
}

void AnimationsSequence::setStartRow(int value)
{
    mStartRow = value;
}
int AnimationsSequence::getEndRow() const
{
    return mEndRow;
}

void AnimationsSequence::setEndRow(int value)
{
    mEndRow = value;
}
int AnimationsSequence::getEndColumn() const
{
    return mEndCol;
}

void AnimationsSequence::setEndColumn(int value)
{
    mEndCol = value;
}

void AnimationsSequence::setStart(int row, int col)
{
    mStartRow = row;
    mStartCol = col;
}

QPair<int, int> AnimationsSequence::getStart() const
{
    return QPair<int,int>(mStartRow, mStartCol);
}

void AnimationsSequence::setEnd(int row, int col)
{
    mEndRow = row;
    mEndCol = col;
}

QPair<int, int> AnimationsSequence::getEnd() const
{
    return QPair<int,int>(mEndRow, mEndCol);
}
QString AnimationsSequence::getName() const
{
    return mName;
}

void AnimationsSequence::setName(const QString &value)
{
    mName = value;
}
int AnimationsSequence::getAnimationSpeed() const
{
    return mAnimationSpeed;
}

void AnimationsSequence::setAnimationSpeed(int value)
{
    mAnimationSpeed = value;
}






