#ifndef FOLDERITEM_H
#define FOLDERITEM_H

#include "items/abstracttreeitem.h"
#include <QIcon>

class FolderItem : public AbstractTreeItem
{
public:
    FolderItem(AbstractTreeItem *parent = 0);
    FolderItem(const QString& name, AbstractTreeItem *parent = 0);

    QString getName() const;
    void setName(const QString &value);

private:
    QString mName;

};

#endif // FOLDERITEM_H
