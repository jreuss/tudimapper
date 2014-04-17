#include "abstracttreeitem.h"

AbstractTreeItem::AbstractTreeItem(AbstractTreeItem *parent) : ID(QUuid::createUuid().toString())
{
     mParent = parent;
     mItemType = AbstractTreeItem::TemplateType;
}

AbstractTreeItem::~AbstractTreeItem()
{
    qDebug() << "fisse";
    qDeleteAll(mChildren);

//    if(mParent!=NULL) {
//        //int index = mParent->rowOfChild(this);
//        mParent->removeChild(this);
//    }
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
    qDebug() << index;
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

QString AbstractTreeItem::getID() const
{
    return ID;
}
QString AbstractTreeItem::getFolderID() const
{
    return mFolderID;
}

void AbstractTreeItem::setFolderID(const QString &value)
{
    mFolderID = value;
}








