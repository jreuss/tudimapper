#ifndef ABSTRACTTREEPIXMAPMODEL_H
#define ABSTRACTTREEPIXMAPMODEL_H

#include <QAbstractItemModel>
#include <items/abstracttreepixmapitem.h>
#include <QDebug>

class AbstractTreePixmapModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AbstractTreePixmapModel(const int &treeDepth, QObject *parent = 0);
    virtual ~AbstractTreePixmapModel();

    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    AbstractTreePixmapItem *getRoot() const;
    void setRoot(AbstractTreePixmapItem *value);

    AbstractTreePixmapItem *itemFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromItem(AbstractTreePixmapItem *item) const;

    void insertItem(int i, AbstractTreePixmapItem *parentItem,
                    AbstractTreePixmapItem *item);
    void insertItem(QModelIndex parentIndex,
                    AbstractTreePixmapItem *item);
    void removeItem(AbstractTreePixmapItem *item);
    void removeItem(const QModelIndex &index);
    void removeAllItems();
    AbstractTreePixmapItem *takeItem(AbstractTreePixmapItem *item);
    int childCount;


protected:
    int rowForItem(AbstractTreePixmapItem *item) const;
    AbstractTreePixmapItem *root;

    //Insert and remove item functions

    const int TREE_DEPTH;
    AbstractTreePixmapItem *mRoot;
};

#endif // ABSTRACTTREEPIXMAPMODEL_H
