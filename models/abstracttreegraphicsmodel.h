#ifndef ABSTRACTTREEGRAPHICSMODEL_H
#define ABSTRACTTREEGRAPHICSMODEL_H

#include "stable_pch.h"
#include "items/abstracttreegraphicsitem.h"

class AbstractTreeGraphicsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AbstractTreeGraphicsModel(const int &treeDepth, QObject *parent = 0);
     ~AbstractTreeGraphicsModel();
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

    AbstractTreeGraphicsItem *itemFromIndex(const QModelIndex &index) const;

    QModelIndex indexFromItem(AbstractTreeGraphicsItem *itm ,AbstractTreeGraphicsItem *root);

    AbstractTreeGraphicsItem *getRoot() const;
    void setRoot(AbstractTreeGraphicsItem *value);

protected:
    const int TREE_DEPTH;
    AbstractTreeGraphicsItem *mRoot;
};

#endif // AbstractTreeGraphicsModel_H
