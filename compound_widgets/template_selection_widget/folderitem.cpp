#include "folderitem.h"

FolderItem::FolderItem(AbstractTreeItem *parent) : AbstractTreeItem(parent)
{

}

FolderItem::FolderItem(const QString &name, AbstractTreeItem *parent) : AbstractTreeItem(parent)
{
    mName = name;
}

QString FolderItem::getName() const
{
    return mName;
}

void FolderItem::setName(const QString &value)
{
    mName = value;
}



