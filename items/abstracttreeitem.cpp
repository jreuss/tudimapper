#include "abstracttreeitem.h"

AbstractTreeItem::AbstractTreeItem(AbstractTreeItem *parent)
{

     mParent = parent;
     mName = "test" + QString::number(qrand()%100);
     mItemType = AbstractTreeItem::TemplateType;
}

AbstractTreeItem::~AbstractTreeItem()
{

   //This is now Done in the removeitem in the model
    //qDeleteAll(mChildren);

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

AbstractTreeItem *AbstractTreeItem::getParent()
{
    return mParent;
}

void AbstractTreeItem::setParent(AbstractTreeItem *value)
{
    mParent = value;
}
QList<AbstractTreeItem *> AbstractTreeItem::getChildren()
{
    return mChildren;
}

void AbstractTreeItem::setChildren(const QList<AbstractTreeItem *> &value)
{
    mChildren = value;
}

void AbstractTreeItem::addChildren(QList<AbstractTreeItem *> children)
{
    foreach(AbstractTreeItem *child, children) {
        child->setParent(this);
        mChildren.append(child);
    }
}

AbstractTreeItem::itemType AbstractTreeItem::getItemType() const
{
    return mItemType;
}

void AbstractTreeItem::setItemType(const AbstractTreeItem::itemType &value)
{
    mItemType = value;
}


QString AbstractTreeItem::name() const
{
    return mName;
}

void AbstractTreeItem::setName(const QString &value)
{
    mName = value;
}









