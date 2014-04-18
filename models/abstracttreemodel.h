#ifndef ABSTRACTTREEMODEL_H
#define ABSTRACTTREEMODEL_H

#include "stable_pch.h"
#include "items/abstracttreeitem.h"

class AbstractTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AbstractTreeModel(const int &treeDepth, QObject *parent = 0);
    virtual ~AbstractTreeModel();

    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    AbstractTreeItem *getRoot() const;
    void setRoot(AbstractTreeItem *value);

    AbstractTreeItem *itemFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromItem(AbstractTreeItem *item) const;

    void insertItem(int i, AbstractTreeItem *parentItem,
                    AbstractTreeItem *item);
    void insertItem(QModelIndex parentIndex,
                    AbstractTreeItem *item);
    void removeItem(AbstractTreeItem *item);
    void removeItem(const QModelIndex &index);
    void removeAllItems();
    int childCount;


protected:
    int rowForItem(AbstractTreeItem *item) const;
    AbstractTreeItem *root;

    //Insert and remove item functions

    const int TREE_DEPTH;
    AbstractTreeItem *mRoot;
};

#endif // ABSTRACTTREEMODEL_H
