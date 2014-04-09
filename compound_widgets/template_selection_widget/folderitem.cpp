#include "folderitem.h"

FolderItem::FolderItem(AbstractTreeItem *parent) : AbstractTreeItem(parent)
{

}
QString FolderItem::getName() const
{
    return mName;
}

void FolderItem::setName(const QString &value)
{
    mName = value;
}

