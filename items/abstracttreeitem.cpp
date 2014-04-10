#include "abstracttreeitem.h"

AbstractTreeItem::AbstractTreeItem(AbstractTreeItem *parent)
{
     mParent = parent;
     mItemType = AbstractTreeItem::TemplateType;
}

AbstractTreeItem::~AbstractTreeItem()
{
    qDeleteAll(mChildren);

    if(mParent!=NULL) {
        int index = mParent->rowOfChild(this);
        (void) mParent->takeChild(index);
    }
}

int AbstractTreeItem::childCount()
{
    return mChildren.count();
}

int AbstractTreeItem::rowOfChild(AbstractTreeItem *child) const
{
    return mChildren.indexOf(child);
}

AbstractTreeItem *AbstractTreeItem::takeChild(int index)
{
    return mChildren.takeAt (index);
}

AbstractTreeItem *AbstractTreeItem::childAt(int index) const
{
    return mChildren.value(index);
}

AbstractTreeItem *AbstractTreeItem::parent() const
{
    return mParent;
}

void AbstractTreeItem::removeChild(AbstractTreeItem *child)
{
    int index = rowOfChild(child);
    mChildren.removeAt(index);
}

void AbstractTreeItem::addChild(AbstractTreeItem *child)
{
    child->setParent(this);
    mChildren.append(child);
}

AbstractTreeItem *AbstractTreeItem::getParent() const
{
    return mParent;
}

void AbstractTreeItem::setParent(AbstractTreeItem *value)
{
    mParent = value;
}
QList<AbstractTreeItem *> AbstractTreeItem::getChildren() const
{
    return mChildren;
}

void AbstractTreeItem::setChildren(const QList<AbstractTreeItem *> &value)
{
    mChildren = value;
}

void AbstractTreeItem::addChildren(QList<AbstractTreeItem *> children)
{
    mChildren.append(children);
}

AbstractTreeItem::itemType AbstractTreeItem::getItemType() const
{
    return mItemType;
}

void AbstractTreeItem::setItemType(const AbstractTreeItem::itemType &value)
{
    mItemType = value;
}






