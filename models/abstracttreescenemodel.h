#ifndef ABSTRACTTREESCENEMODEL_H
#define ABSTRACTTREESCENEMODEL_H

#include <QAbstractItemModel>
#include "items/abstractsceneitem.h"

class AbstractTreeSceneModel : public QAbstractItemModel
{
    Q_OBJECT
public:
public:
    explicit AbstractTreeSceneModel(const int &treeDepth, QObject *parent = 0);
    virtual ~AbstractTreeSceneModel();

    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    AbstractSceneItem *getRoot() const;
    void setRoot(AbstractSceneItem *value);

    AbstractSceneItem *itemFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromItem(AbstractSceneItem *item) const;

    void insertItem(int i, AbstractSceneItem *parentItem,
                    AbstractSceneItem *item);
    void insertItem(QModelIndex parentIndex,
                    AbstractSceneItem *item);
    void removeItem(AbstractSceneItem *item);
    void removeItem(const QModelIndex &index);
    void removeAllItems();
    int childCount;


protected:
    int rowForItem(AbstractSceneItem *item) const;
    AbstractSceneItem *root;

    //Insert and remove item functions

    const int TREE_DEPTH;
    AbstractSceneItem *mRoot;

signals:

public slots:

};

#endif // ABSTRACTTREESCENEMODEL_H
