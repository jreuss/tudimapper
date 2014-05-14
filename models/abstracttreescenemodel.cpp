#include "abstracttreescenemodel.h"

AbstractTreeSceneModel::AbstractTreeSceneModel(const int &treeDepth, QObject *parent) :
    QAbstractItemModel(parent), TREE_DEPTH(treeDepth)
{
    mRoot = new AbstractSceneItem(NULL);
}

AbstractTreeSceneModel::~AbstractTreeSceneModel()
{

}

QModelIndex AbstractTreeSceneModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row, column, parent)) {
        AbstractSceneItem *parentItem = itemFromIndex(parent);
        AbstractSceneItem *childItem = parentItem->mChildren.at(row);
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex AbstractTreeSceneModel::parent(const QModelIndex &child) const
{
    AbstractSceneItem *childItem = itemFromIndex(child);
    AbstractSceneItem *parentItem = childItem->mParent;

    if(parentItem == mRoot) {
        return QModelIndex();
    }

    int row = rowForItem(parentItem);
    int column = 0;
    return createIndex(row, column, parentItem);
}

int AbstractTreeSceneModel::rowCount(const QModelIndex &parent) const
{
    AbstractSceneItem *parentItem = itemFromIndex(parent);
    return parentItem ? parentItem->mChildren.count() : 0;
}

int AbstractTreeSceneModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return TREE_DEPTH;
}

AbstractSceneItem *AbstractTreeSceneModel::itemFromIndex(const QModelIndex &index) const
{
    if(index.isValid()) {
        return static_cast<AbstractSceneItem*>(index.internalPointer());
    }
    return mRoot;
}

int AbstractTreeSceneModel::rowForItem(AbstractSceneItem *item) const
{
    return item->mParent->mChildren.indexOf(item);
}

QModelIndex AbstractTreeSceneModel::indexFromItem(AbstractSceneItem *item) const
{
    if(item->mParent) {
        int row = item->mParent->mChildren.indexOf(item);

        if(row != -1) {
            return createIndex(row, 0, item);
        }
    }

    return QModelIndex();
}

AbstractSceneItem *AbstractTreeSceneModel::getRoot() const
{
    return mRoot;
}

void AbstractTreeSceneModel::setRoot(AbstractSceneItem *value)
{
    mRoot = value;
}

//Insert and Remove item functions
void AbstractTreeSceneModel::insertItem(int i, AbstractSceneItem *parentItem, AbstractSceneItem *item)
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

void AbstractTreeSceneModel::insertItem(QModelIndex parentIndex, AbstractSceneItem *item)
{
    AbstractSceneItem *parentItem = itemFromIndex(parentIndex);

    int firstRow = parentItem->mChildren.count();
    int lastRow = firstRow;
    beginInsertRows(parentIndex, firstRow, lastRow);
    parentItem->mChildren.append(item);
    item->mParent = parentItem;
    childCount++;
    endInsertRows();
}

void AbstractTreeSceneModel::removeItem(AbstractSceneItem *item)
{
    AbstractSceneItem *parentItem = item->mParent;
    const QModelIndex parentIndex = indexFromItem(parentItem);
    foreach(AbstractSceneItem *child, item->mChildren){
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

void AbstractTreeSceneModel::removeItem(const QModelIndex &index)
{
    AbstractSceneItem *itemToRemove = itemFromIndex(index);
    foreach(AbstractSceneItem *child, itemToRemove->mChildren){
        removeItem(indexFromItem(child));
    }

    if(itemToRemove->mParent) {
        AbstractSceneItem *parentItem = itemToRemove->mParent;
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

void AbstractTreeSceneModel::removeAllItems()
{
    beginResetModel();
    mRoot->mChildren.clear();
    resetInternalData();
    endResetModel();
}
