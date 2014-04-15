#include "folderitem.h"
#include "utility/globals.h"

FolderItem::FolderItem(AbstractTreeItem *parent) : AbstractTreeItem(parent)
{
    mFolderRoot = new AbstractTreeItem();
    mFolderRoot->setItemType(AbstractTreeItem::FolderType);
    mItemType = AbstractTreeItem::FolderType;
}

FolderItem::FolderItem(const QString &name, AbstractTreeItem *parent) : AbstractTreeItem(parent)
{
    mName = name;
    mFolderRoot = new AbstractTreeItem();
    mFolderRoot->setItemType(AbstractTreeItem::FolderType);
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

void FolderItem::switchItemToThisFolder(AbstractTreeItem *child)
{
    // get the folder that is currently containing the child
    FolderItem *currFolder = globs::folderHash.value(child->getFolderID());

    // now remove the child from that folder
    currFolder->getFolderRoot()->removeChild(child);

    // and finally add it to this folder
    child->setFolderID(this->getID());
    this->getFolderRoot()->addChild(child);
}

AbstractTreeItem *FolderItem::getFolderRoot() const
{
    return mFolderRoot;
}

void FolderItem::setFolderRoot(AbstractTreeItem *value)
{
    mFolderRoot = value;
}




