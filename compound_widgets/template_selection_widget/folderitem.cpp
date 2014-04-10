#include "folderitem.h"

FolderItem::FolderItem(AbstractTreeItem *parent) : AbstractTreeItem(parent)
{
    mFolderRoot = new ItemTemplate(QString());
    mItemType = AbstractTreeItem::FolderType;
}

FolderItem::FolderItem(const QString &name, AbstractTreeItem *parent) : AbstractTreeItem(parent)
{
    mName = name;
    mFolderRoot = new ItemTemplate(QString());
    mItemType = AbstractTreeItem::FolderType;
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




