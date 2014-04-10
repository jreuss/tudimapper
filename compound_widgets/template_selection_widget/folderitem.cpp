#include "folderitem.h"

FolderItem::FolderItem(AbstractTreeItem *parent) : AbstractTreeItem(parent)
{
    mFolderRoot = new ItemTemplate(QString());
}

FolderItem::FolderItem(const QString &name, AbstractTreeItem *parent) : AbstractTreeItem(parent)
{
    mName = name;
    mFolderRoot = new ItemTemplate(QString());
}

QString FolderItem::getName() const
{
    return mName;
}

void FolderItem::setName(const QString &value)
{
    mName = value;
}
ItemTemplate *FolderItem::getFolderRoot() const
{
    return mFolderRoot;
}

void FolderItem::setFolderRoot(ItemTemplate *value)
{
    mFolderRoot = value;
}




