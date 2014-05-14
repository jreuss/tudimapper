#include "abstractsceneitem.h"

AbstractSceneItem::AbstractSceneItem(AbstractSceneItem* parent)
{
    mParent = parent;
}

AbstractSceneItem::~AbstractSceneItem()
{

}

int AbstractSceneItem::childCount()
{
    return mChildren.count();
}

int AbstractSceneItem::rowOfChild(AbstractSceneItem *child) const
{
    return mChildren.indexOf(child);
}

AbstractSceneItem *AbstractSceneItem::takeChild(int index)
{
    return mChildren.takeAt (index);
}

AbstractSceneItem *AbstractSceneItem::childAt(int index) const
{
    return mChildren.value(index);
}

AbstractSceneItem *AbstractSceneItem::parent() const
{
    return mParent;
}

void AbstractSceneItem::addChild(AbstractSceneItem *child)
{
    child->setParent(this);
    mChildren.append(child);
}

AbstractSceneItem *AbstractSceneItem::getParent() const
{
    return mParent;
}

void AbstractSceneItem::setParent(AbstractSceneItem *value)
{
    mParent = value;
}
QList<AbstractSceneItem *> AbstractSceneItem::getChildren() const
{
    return mChildren;
}
