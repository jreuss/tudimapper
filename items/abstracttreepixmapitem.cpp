#include "abstracttreepixmapitem.h"

AbstractTreePixmapItem::AbstractTreePixmapItem(AbstractTreePixmapItem* parent)
{
    mParent = parent;
}

AbstractTreePixmapItem::~AbstractTreePixmapItem()
{

}

int AbstractTreePixmapItem::childCount()
{
    return mChildren.count();
}

int AbstractTreePixmapItem::rowOfChild(AbstractTreePixmapItem *child) const
{
    return mChildren.indexOf(child);
}

AbstractTreePixmapItem *AbstractTreePixmapItem::takeChild(int index)
{
    return mChildren.takeAt (index);
}

AbstractTreePixmapItem *AbstractTreePixmapItem::childAt(int index) const
{
    return mChildren.value(index);
}

AbstractTreePixmapItem *AbstractTreePixmapItem::parent() const
{
    return mParent;
}

void AbstractTreePixmapItem::addChild(AbstractTreePixmapItem *child)
{
    child->setParent(this);
    mChildren.append(child);
}

AbstractTreePixmapItem *AbstractTreePixmapItem::getParent() const
{
    return mParent;
}

void AbstractTreePixmapItem::setParent(AbstractTreePixmapItem *value)
{
    mParent = value;
}
QList<AbstractTreePixmapItem *> AbstractTreePixmapItem::getChildren() const
{
    return mChildren;
}

