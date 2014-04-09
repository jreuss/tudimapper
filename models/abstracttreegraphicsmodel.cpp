#include "abstracttreegraphicsmodel.h"

AbstractTreeGraphicsModel::AbstractTreeGraphicsModel(const int &treeDepth, QObject *parent) :
    QAbstractItemModel(parent), TREE_DEPTH(treeDepth)
{
    mRoot = new AbstractTreeGraphicsItem();
}

AbstractTreeGraphicsModel::~AbstractTreeGraphicsModel()
{
    delete mRoot;
}

int AbstractTreeGraphicsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return TREE_DEPTH;
}

int AbstractTreeGraphicsModel::rowCount(const QModelIndex &parent) const
{
    // we only allow children in the first coloumn (:0)
    if (parent.column() > 0 && parent.isValid()) {
        return 0;
    }

    AbstractTreeGraphicsItem *parentItem = itemFromIndex (parent);

    /* if the item has a parent we return the index of the item
     * in the parents, children. if there is no parent, then it
     * must be root */
    return parentItem ? parentItem->childCount() : 0;
}

bool AbstractTreeGraphicsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(!mRoot) {
        mRoot = new AbstractTreeGraphicsItem();
    }

    AbstractTreeGraphicsItem *item = parent.isValid () ?
                itemFromIndex (parent) : mRoot;

    beginInsertRows (parent, row, row+count-1);

    for(int i=0; i < count; ++i) {
        (void) new AbstractTreeGraphicsItem(item);
    }

    endInsertRows ();

    return true;
}

bool AbstractTreeGraphicsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(!mRoot) {
        return false;
    }

    AbstractTreeGraphicsItem *item = parent.isValid () ?
                itemFromIndex (parent) : mRoot;

    beginRemoveRows (parent, row, row+count-1);

    for(int i=0; i < count; ++i) {
        delete item->takeChild (row);
    }

    endRemoveRows ();

    return true;
}

QModelIndex AbstractTreeGraphicsModel::index(int row, int column, const QModelIndex &parent) const
{
    /* if the parents column is not 0, then no child is allowed
     * and therefore no index is created */
    if(!mRoot || row < 0 || column < 0 || column >= TREE_DEPTH
            || (parent.isValid() && parent.column() != 0)) {
        return QModelIndex(); // invalid
    }

    AbstractTreeGraphicsItem *parentItem = itemFromIndex(parent);

    Q_ASSERT(parentItem); // must exist

    // retrieve parents row-th child
    if(AbstractTreeGraphicsItem *item = parentItem->childAt (row)) {
        /* find the item and create QModelIndex with a pointer to
             * the child (: QModelIndex allows *void) */
        return createIndex(row, column, item);
    }

    return QModelIndex(); // invalid
}

QModelIndex AbstractTreeGraphicsModel::parent(const QModelIndex &child) const
{
    if(!child.isValid ()) {
        return QModelIndex(); // invalid
    }

    /* we need the index of the childs parent, in it's parent
     * (:grandparents) child array */
    if(AbstractTreeGraphicsItem *item = itemFromIndex (child)) {
        if(AbstractTreeGraphicsItem *parentItem = item->parent ()) {
            if(parentItem == mRoot) {
                return QModelIndex();
            }
            if(AbstractTreeGraphicsItem *grandParentItem =
                    parentItem->parent ()) {
                int row = grandParentItem->rowOfChild (parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }

    return QModelIndex(); // invalid
}

AbstractTreeGraphicsItem *AbstractTreeGraphicsModel::itemFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        if(AbstractTreeGraphicsItem *item =
                static_cast<AbstractTreeGraphicsItem *>(index.internalPointer ())) {
            return item;
        }
    }

    return mRoot;
}

QModelIndex AbstractTreeGraphicsModel::indexFromItem(AbstractTreeGraphicsItem *itm, AbstractTreeGraphicsItem *root)
{
    foreach(AbstractTreeGraphicsItem *child, root->getChildren()) {
        if(itm == child) {
            if( AbstractTreeGraphicsItem *parentItem = child->parent()){
                int row = parentItem->rowOfChild (child);
                return createIndex(row, 0, child);
            }
        } else {
            indexFromItem(itm,child);
        }
    }
    return QModelIndex();
}

AbstractTreeGraphicsItem *AbstractTreeGraphicsModel::getRoot() const
{
    return mRoot;
}

void AbstractTreeGraphicsModel::setRoot(AbstractTreeGraphicsItem *value)
{
    mRoot = value;
}





