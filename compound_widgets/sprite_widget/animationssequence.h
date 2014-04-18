#ifndef ANIMATIONSSEQUENCE_H
#define ANIMATIONSSEQUENCE_H

#include "stable_pch.h"
#include "items/abstracttreeitem.h"

class AnimationsSequence : public AbstractTreeItem
{
public:
    AnimationsSequence(AbstractTreeItem *parent);
    AnimationsSequence(const QString &mName, int startRow=0, int startCol=0,
                       int endRow=0, int endCol=0, int animationSpeed=150, AbstractTreeItem *parent=0);

    int getStartColumn() const;
    void setStartColumn(int value);

    int getStartRow() const;
    void setStartRow(int value);

    int getEndRow() const;
    void setEndRow(int value);

    int getEndColumn() const;
    void setEndColumn(int value);

    void setStart(int row, int col);
    QPair<int,int> getStart() const;

    void setEnd(int row,int col);
    QPair<int,int> getEnd() const;

    QString getName() const;
    void setName(const QString &value);

    int getAnimationSpeed() const;
    void setAnimationSpeed(int value);

private:
    QString mName;
    int mStartCol, mStartRow, mEndRow, mEndCol, mAnimationSpeed;

    
};

#endif // ANIMATIONSSEQUENCE_H
