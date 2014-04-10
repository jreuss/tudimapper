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

    QString getName() const;
    void setName(const QString &value);

    ItemTemplate *getFolderRoot() const;
    void setFolderRoot(ItemTemplate *value);

private:
    QString mName;
    ItemTemplate *mFolderRoot;
};

#endif // FOLDERITEM_H
