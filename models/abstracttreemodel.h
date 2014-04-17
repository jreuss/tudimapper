#ifndef ABSTRACTTREEMODEL_H
#define ABSTRACTTREEMODEL_H

#include <QAbstractItemModel>
#include <QDebug>
#include "items/abstracttreeitem.h"

class AbstractTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AbstractTreeModel(const int &treeDepth, QObject *parent = 0);
    virtual ~AbstractTreeModel();
    int columnCount (const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows (int row, int count, const QModelIndex &parent);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    bool setHeaderData (int section __attribute__((unused)),
                           Qt::Orientation orientation __attribute__((unused)),
                           const QVariant &value __attribute__((unused)),
                           int role __attribute__((unused)))
       { return false;}

    AbstractTreeItem *itemFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromItem(AbstractTreeItem *itm);

    void reset();

    AbstractTreeItem *getRoot() const;
    void setRoot(AbstractTreeItem *value);

protected:
    const int TREE_DEPTH;
    AbstractTreeItem *mRoot;
};

#endif // ABSTRACTTREEMODEL_H
