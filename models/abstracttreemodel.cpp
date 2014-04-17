#include "abstracttreemodel.h"

AbstractTreeModel::AbstractTreeModel(const int &treeDepth, QObject *parent) :
    QAbstractItemModel(parent), TREE_DEPTH(treeDepth)
{
    mRoot = new AbstractTreeItem();
}

AbstractTreeModel::~AbstractTreeModel()
{
    //qDebug() << "dtor abstracttreemodel called";
    //delete mRoot;
}

int AbstractTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return TREE_DEPTH;
}

int AbstractTreeModel::rowCount(const QModelIndex &parent) const
{
    // we only allow children in the first coloumn (:0)
    if (parent.column() > 0 && parent.isValid()) {
        return 0;
    }

    AbstractTreeItem *parentItem = itemFromIndex (parent);

    /* if the item has a parent we return the index of the item
     * in the parents, children. if there is no parent, then it
     * must be root */
    return parentItem ? parentItem->childCount() : 0;
}

bool AbstractTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(!mRoot) {
        mRoot = new AbstractTreeItem();
    }

    AbstractTreeItem *item = parent.isValid () ?
                itemFromIndex (parent) : mRoot;

    beginInsertRows (parent, row, row+count-1);

    for(int i=0; i < count; ++i) {
        (void) new AbstractTreeItem(item);
    }

    endInsertRows ();

    return true;
}

bool AbstractTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(!mRoot) {
        return false;
    }

    AbstractTreeItem *item = parent.isValid () ?
                itemFromIndex (parent) : mRoot;

    beginRemoveRows (parent, row, row+count-1);

    for(int i=0; i < count; ++i) {
        delete item->takeChild (row);
    }

    endRemoveRows ();

    return true;
}

QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    /* if the parents column is not 0, then no child is allowed
     * and therefore no index is created */
    if(!mRoot || row < 0 || column < 0 || column >= TREE_DEPTH
            || (parent.isValid() && parent.column() != 0)) {
        return QModelIndex(); // invalid
    }

    AbstractTreeItem *parentItem = itemFromIndex(parent);

    Q_ASSERT(parentItem); // must exist

    // retrieve parents row-th child
    if(AbstractTreeItem *item = parentItem->childAt (row)) {
        /* find the item and create QModelIndex with a pointer to
             * the child (: QModelIndex allows *void) */
        return createIndex(row, column, item);
    }

    return QModelIndex(); // invalid
}

QModelIndex AbstractTreeModel::parent(const QModelIndex &child) const
{
    if(!child.isValid ()) {
        return QModelIndex(); // invalid
    }

    /* we need the index of the childs parent, in it's parent
     * (:grandparents) child array */
    if(AbstractTreeItem *item = itemFromIndex (child)) {
        if(AbstractTreeItem *parentItem = item->getParent ()) {

            if(parentItem == mRoot) {
                return QModelIndex();
            }

            if(AbstractTreeItem *grandParentItem =
                    parentItem->getParent ()) {
                int row = grandParentItem->rowOfChild (parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }

    return QModelIndex(); // invalid
}

AbstractTreeItem *AbstractTreeModel::itemFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        if(AbstractTreeItem *item =
                static_cast<AbstractTreeItem *>(index.internalPointer ())) {
            return item;
        }
    }

    return mRoot;
}

QModelIndex AbstractTreeModel::indexFromItem(AbstractTreeItem *itm)
{
    if( AbstractTreeItem *parentItem = itm->getParent()){
        int row = parentItem->rowOfChild (itm);
        return createIndex(row, 0, itm);
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





