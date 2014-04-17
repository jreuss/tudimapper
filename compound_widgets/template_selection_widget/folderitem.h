#ifndef FOLDERITEM_H
#define FOLDERITEM_H

#include "items/abstracttreeitem.h"
#include "items/itemtemplate.h"

#include <QIcon>

class FolderItem : public AbstractTreeItem
{

public:
    FolderItem(AbstractTreeItem *parent = 0);
    FolderItem(const QString& name, AbstractTreeItem *parent = 0);

    ~FolderItem();

    QString getName() const;
    void setName(const QString &value);

    void switchItemToThisFolder(AbstractTreeItem *child);

    AbstractTreeItem *getFolderRoot() const;
    void setFolderRoot(AbstractTreeItem *value);

private:
    QString mName;

    AbstractTreeItem *mFolderRoot;
};

#endif // FOLDERITEM_H
