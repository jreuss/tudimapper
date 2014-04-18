#ifndef ABSTRACTTREEITEM_H
#define ABSTRACTTREEITEM_H

#include "stable_pch.h"

class AbstractTreeItem
{
public:
    enum itemType { TemplateType, FolderType };
    AbstractTreeItem(AbstractTreeItem* parent = 0);

    virtual ~AbstractTreeItem();
    int childCount();
    int rowOfChild(AbstractTreeItem *child) const;
    AbstractTreeItem *takeChild(int index);
    AbstractTreeItem *childAt(int index) const;

    void removeChild(AbstractTreeItem *child);
    void addChild(AbstractTreeItem *child);

    AbstractTreeItem *getParent();
    void setParent(AbstractTreeItem *value);

    QList<AbstractTreeItem *> getChildren();
    void setChildren(const QList<AbstractTreeItem *> &value);

    void addChildren(QList<AbstractTreeItem *> children);

    AbstractTreeItem::itemType getItemType() const;
    void setItemType(const AbstractTreeItem::itemType &value);

    QString getID() const;

    QString getFolderID() const;
    void setFolderID(const QString &value);

    QString name() const;
    void setName(const QString &value);

    AbstractTreeItem *mParent;
    QList<AbstractTreeItem *> mChildren;
protected:


    itemType mItemType;
    //const QString ID;
    QString mName;

};

#endif // ABSTRACTTREEITEM_H
