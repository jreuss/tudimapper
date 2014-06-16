#include "abstracttreepixmapmodel.h"

AbstractTreePixmapModel::AbstractTreePixmapModel(const int &treeDepth, QObject *parent) :
    QAbstractItemModel(parent), TREE_DEPTH(treeDepth)
{
    mRoot = new AbstractTreePixmapItem(NULL);
}

AbstractTreePixmapModel::~AbstractTreePixmapModel()
{

}

QModelIndex AbstractTreePixmapModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row, column, parent)) {
        AbstractTreePixmapItem *parentItem = itemFromIndex(parent);
        AbstractTreePixmapItem *childItem = parentItem->mChildren.at(row);
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex AbstractTreePixmapModel::parent(const QModelIndex &child) const
{
    AbstractTreePixmapItem *childItem = itemFromIndex(child);
    AbstractTreePixmapItem *parentItem = childItem->mParent;

    if(parentItem == mRoot) {
        return QModelIndex();
    }

    int row = rowForItem(parentItem);
    int column = 0;
    return createIndex(row, column, parentItem);
}

int AbstractTreePixmapModel::rowCount(const QModelIndex &parent) const
{
    AbstractTreePixmapItem *parentItem = itemFromIndex(parent);
    return parentItem ? parentItem->mChildren.count() : 0;
}

int AbstractTreePixmapModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return TREE_DEPTH;
}

AbstractTreePixmapItem *AbstractTreePixmapModel::itemFromIndex(const QModelIndex &index) const
{
    if(index.isValid()) {
        return static_cast<AbstractTreePixmapItem*>(index.internalPointer());
    }
    return mRoot;
}

int AbstractTreePixmapModel::rowForItem(AbstractTreePixmapItem *item) const
{
    return item->mParent->mChildren.indexOf(item);
}

QModelIndex AbstractTreePixmapModel::indexFromItem(AbstractTreePixmapItem *item) const
{
    if(item->mParent) {
        int row = item->mParent->mChildren.indexOf(item);

        if(row != -1) {
            return createIndex(row, 0, item);
        }
    }

    return QModelIndex();
}

AbstractTreePixmapItem *AbstractTreePixmapModel::getRoot() const
{
    return mRoot;
}

void AbstractTreePixmapModel::setRoot(AbstractTreePixmapItem *value)
{
    mRoot = value;
}

//Insert and Remove item functions
void AbstractTreePixmapModel::insertItem(int i, AbstractTreePixmapItem *parentItem, AbstractTreePixmapItem *item)
{
    const QModelIndex parent = indexFromItem(parentItem);
    int firstRow = i;
    int lastRow = i;

    beginInsertRows(parent, firstRow, lastRow);
    parentItem->mChildren.insert(i, item);
    item->mParent = parentItem;
    childCount++;
    endInsertRows();
}

void AbstractTreePixmapModel::insertItem(QModelIndex parentIndex, AbstractTreePixmapItem *item)
{
    AbstractTreePixmapItem *parentItem = itemFromIndex(parentIndex);

    int firstRow = parentItem->mChildren.count();
    int lastRow = firstRow;
    beginInsertRows(parentIndex, firstRow, lastRow);
    parentItem->mChildren.append(item);
    item->mParent = parentItem;
    childCount++;
    endInsertRows();
}

void AbstractTreePixmapModel::removeItem(AbstractTreePixmapItem *item)
{
    AbstractTreePixmapItem *parentItem = item->mParent;
    const QModelIndex parentIndex = indexFromItem(parentItem);
    foreach(AbstractTreePixmapItem *child, item->mChildren){
        removeItem(indexFromItem(child));
    }

    int i = rowForItem(item);
    int firstRow = i;
    int lastRow = i;

    beginRemoveRows(parentIndex, firstRow, lastRow);
    delete parentItem->mChildren.takeAt(i);
    childCount--;
    endRemoveRows();

}

AbstractTreePixmapItem *AbstractTreePixmapModel::takeItem(AbstractTreePixmapItem *item)
{
    AbstractTreePixmapItem *parentItem = item->mParent;
    const QModelIndex parentIndex = indexFromItem(parentItem);

    int i = rowForItem(item);
    int firstRow = i;
    int lastRow = i;

    AbstractTreePixmapItem *taken;

    beginRemoveRows(parentIndex, firstRow, lastRow);
    taken = parentItem->mChildren.takeAt(i);
    childCount--;
    endRemoveRows();

    return taken;
}

void AbstractTreePixmapModel::removeItem(const QModelIndex &index)
{
    AbstractTreePixmapItem *itemToRemove = itemFromIndex(index);
    foreach(AbstractTreePixmapItem *child, itemToRemove->mChildren){
        removeItem(indexFromItem(child));
    }

    if(itemToRemove->mParent) {
        AbstractTreePixmapItem *parentItem = itemToRemove->mParent;
        QModelIndex parentIndex = indexFromItem(parentItem);

        int i = parentItem->mChildren.indexOf(itemToRemove);
        int firstRow = i;
        int lastRow = i;

        beginRemoveRows(parentIndex, firstRow, lastRow);
        delete parentItem->mChildren.takeAt(i);
        childCount--;
        endRemoveRows();
    } else {
        int i = mRoot->mChildren.indexOf(itemToRemove);

        if(i != -1) {
            int firstRow = i;
            int lastRow = i;

            beginRemoveRows(QModelIndex(), firstRow, lastRow);
            delete mRoot->mChildren.takeAt(i);
            childCount--;
            endRemoveRows();
        }
    }
}

void AbstractTreePixmapModel::removeAllItems()
{
    beginResetModel();
    mRoot->mChildren.clear();
    resetInternalData();
    endResetModel();
}
