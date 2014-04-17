#include "abstracttreemodel.h"

AbstractTreeModel::AbstractTreeModel(const int &treeDepth, QObject *parent) :
    QAbstractItemModel(parent), TREE_DEPTH(treeDepth)
{
    mRoot = new AbstractTreeItem(NULL);
}

AbstractTreeModel::~AbstractTreeModel()
{
    //qDebug() << "dtor abstracttreemodel called";
    //delete mmRoot;
}

QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row, column, parent)) {
        AbstractTreeItem *parentItem = itemFromIndex(parent);
        AbstractTreeItem *childItem = parentItem->getChildren().at(row);
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex AbstractTreeModel::parent(const QModelIndex &child) const
{
    AbstractTreeItem *childItem = itemFromIndex(child);
    AbstractTreeItem *parentItem = childItem->getParent();

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
    return parentItem ? parentItem->getChildren().count() : 0;
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
    return item->getParent()->getChildren().indexOf(item);
}

QModelIndex AbstractTreeModel::indexFromItem(AbstractTreeItem *item) const
{
    if(item->getParent()) {
        int row = item->getParent()->getChildren().indexOf(item);

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
    parentItem->getChildren().insert(i, item);
    item->setParent(parentItem);
    childCount++;
    endInsertRows();
}

void AbstractTreeModel::insertItem(QModelIndex parentIndex, AbstractTreeItem *item)
{
    AbstractTreeItem *parentItem = itemFromIndex(parentIndex);

    int firstRow = parentItem->getChildren().count();
    int lastRow = firstRow;

    beginInsertRows(parentIndex, firstRow, lastRow);
    parentItem->getChildren().append(item);
    item->setParent(parentItem);
    childCount++;
    endInsertRows();
}

void AbstractTreeModel::removeItem(AbstractTreeItem *item)
{
    AbstractTreeItem *parentItem = item->getParent();
    const QModelIndex parentIndex = indexFromItem(parentItem);

    int i = rowForItem(item);
    int firstRow = i;
    int lastRow = i;

    beginRemoveRows(parentIndex, firstRow, lastRow);
    delete parentItem->getChildren().takeAt(i);
    childCount--;
    endRemoveRows();
}

void AbstractTreeModel::removeItem(const QModelIndex &index)
{
    AbstractTreeItem *itemToRemove = itemFromIndex(index);

    if(itemToRemove->getParent()) {
        AbstractTreeItem *parentItem = itemToRemove->getParent();
        QModelIndex parentIndex = indexFromItem(parentItem);

        int i = parentItem->getChildren().indexOf(itemToRemove);
        int firstRow = i;
        int lastRow = i;

        beginRemoveRows(parentIndex, firstRow, lastRow);
        delete parentItem->getChildren().takeAt(i);
        childCount--;
        endRemoveRows();
    } else {
        int i = mRoot->getChildren().indexOf(itemToRemove);

        if(i != -1) {
            int firstRow = i;
            int lastRow = i;

            beginRemoveRows(QModelIndex(), firstRow, lastRow);
            delete mRoot->getChildren().takeAt(i);
            childCount--;
            endRemoveRows();
        }
    }

}

void AbstractTreeModel::removeAllItems()
{
    beginResetModel();
    mRoot->getChildren().clear();
    resetInternalData();
    endResetModel();
}





