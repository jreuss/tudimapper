#include "abstracttreemodel.h"

AbstractTreeModel::AbstractTreeModel(const int &treeDepth, QObject *parent) :
    QAbstractItemModel(parent), TREE_DEPTH(treeDepth)
{
    mRoot = new AbstractTreeItem(NULL);
}

AbstractTreeModel::~AbstractTreeModel()
{

}

QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row, column, parent)) {
        AbstractTreeItem *parentItem = itemFromIndex(parent);
        AbstractTreeItem *childItem = parentItem->mChildren.at(row);
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex AbstractTreeModel::parent(const QModelIndex &child) const
{
    AbstractTreeItem *childItem = itemFromIndex(child);
    AbstractTreeItem *parentItem = childItem->mParent;

    if(parentItem == mRoot) {
        return QModelIndex();
    }

    int row = rowForItem(parentItem);
    int column = 0;
    return createIndex(row, column, parentItem);
}

int AbstractTreeModel::rowCount(const QModelIndex &parent) const
{
    AbstractTreeItem *parentItem = itemFromIndex(parent);
    return parentItem ? parentItem->mChildren.count() : 0;
}

int AbstractTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return TREE_DEPTH;
}

AbstractTreeItem *AbstractTreeModel::itemFromIndex(const QModelIndex &index) const
{
    if(index.isValid()) {
        return static_cast<AbstractTreeItem*>(index.internalPointer());
    }
    return mRoot;
}

int AbstractTreeModel::rowForItem(AbstractTreeItem *item) const
{
    return item->mParent->mChildren.indexOf(item);
}

QModelIndex AbstractTreeModel::indexFromItem(AbstractTreeItem *item) const
{
    if(item->mParent) {
        int row = item->mParent->mChildren.indexOf(item);

        if(row != -1) {
            return createIndex(row, 0, item);
        }
    }

    return QModelIndex();
}

AbstractTreeItem *AbstractTreeModel::getRoot() const
{
    return mRoot;
}

void AbstractTreeModel::setRoot(AbstractTreeItem *value)
{
    mRoot = value;
}

//Insert and Remove item functions
void AbstractTreeModel::insertItem(int i, AbstractTreeItem *parentItem, AbstractTreeItem *item)
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

void AbstractTreeModel::insertItem(QModelIndex parentIndex, AbstractTreeItem *item)
{
    AbstractTreeItem *parentItem = itemFromIndex(parentIndex);

    int firstRow = parentItem->mChildren.count();
    int lastRow = firstRow;

    beginInsertRows(parentIndex, firstRow, lastRow);
    parentItem->mChildren.append(item);
    item->mParent = parentItem;
    childCount++;
    endInsertRows();
}

void AbstractTreeModel::removeItem(AbstractTreeItem *item)
{
    AbstractTreeItem *parentItem = item->mParent;
    const QModelIndex parentIndex = indexFromItem(parentItem);
    foreach(AbstractTreeItem *child, item->mChildren){
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

void AbstractTreeModel::removeItem(const QModelIndex &index)
{
    AbstractTreeItem *itemToRemove = itemFromIndex(index);
    foreach(AbstractTreeItem *child, itemToRemove->mChildren){
        removeItem(indexFromItem(child));
    }

    if(itemToRemove->mParent) {
        AbstractTreeItem *parentItem = itemToRemove->mParent;
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

void AbstractTreeModel::removeAllItems()
{
    beginResetModel();
    mRoot->mChildren.clear();
    resetInternalData();
    endResetModel();
}





