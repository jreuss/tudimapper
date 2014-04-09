#include "abstracttreegraphicsitem.h"

AbstractTreeGraphicsItem::AbstractTreeGraphicsItem(AbstractTreeGraphicsItem* parent)
{
    mParent = parent;
}

AbstractTreeGraphicsItem::~AbstractTreeGraphicsItem()
{
    qDeleteAll(mChildren);

    if(mParent!=NULL) {
        int index = mParent->rowOfChild(this);
        (void) mParent->takeChild(index);
    }
}

int AbstractTreeGraphicsItem::childCount()
{
    return mChildren.count();
}

int AbstractTreeGraphicsItem::rowOfChild(AbstractTreeGraphicsItem *child) const
{
    return mChildren.indexOf(child);
}

AbstractTreeGraphicsItem *AbstractTreeGraphicsItem::takeChild(int index)
{
    return mChildren.takeAt (index);
}

AbstractTreeGraphicsItem *AbstractTreeGraphicsItem::childAt(int index) const
{
    return mChildren.value(index);
}

AbstractTreeGraphicsItem *AbstractTreeGraphicsItem::parent() const
{
    return mParent;
}

void AbstractTreeGraphicsItem::addChild(AbstractTreeGraphicsItem *child)
{
    child->setParent(this);
    mChildren.append(child);
}

AbstractTreeGraphicsItem *AbstractTreeGraphicsItem::getParent() const
{
    return mParent;
}

void AbstractTreeGraphicsItem::setParent(AbstractTreeGraphicsItem *value)
{
    mParent = value;
}
QList<AbstractTreeGraphicsItem *> AbstractTreeGraphicsItem::getChildren() const
{
    return mChildren;
}





