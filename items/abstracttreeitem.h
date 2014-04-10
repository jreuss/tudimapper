#ifndef ABSTRACTTREEITEM_H
#define ABSTRACTTREEITEM_H

#include <QList>
#include <QDebug>

class AbstractTreeItem
{
public:
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

private:
    AbstractTreeItem *mParent;
    QList<AbstractTreeItem *> mChildren;

};

#endif // ABSTRACTTREEITEM_H
