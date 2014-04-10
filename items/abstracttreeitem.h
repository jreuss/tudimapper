#ifndef ABSTRACTTREEITEM_H
#define ABSTRACTTREEITEM_H

#include <QList>
#include <QDebug>

class AbstractTreeItem
{
public:
    enum itemType { TemplateType, FolderType };
    AbstractTreeItem(AbstractTreeItem* parent = 0);

    ~AbstractTreeItem();
    int childCount();
    int rowOfChild(AbstractTreeItem *child) const;
    AbstractTreeItem *takeChild(int index);
    AbstractTreeItem *childAt(int index) const;
    AbstractTreeItem *parent() const;

    void removeChild(AbstractTreeItem *child);
    void addChild(AbstractTreeItem *child);

    AbstractTreeItem *getParent() const;
    void setParent(AbstractTreeItem *value);

    QList<AbstractTreeItem *> getChildren() const;
    void setChildren(const QList<AbstractTreeItem *> &value);

    void addChildren(QList<AbstractTreeItem *> children);


    AbstractTreeItem::itemType getItemType() const;
    void setItemType(const AbstractTreeItem::itemType &value);

protected:
    AbstractTreeItem *mParent;
    QList<AbstractTreeItem *> mChildren;
    itemType mItemType;

};

#endif // ABSTRACTTREEITEM_H
