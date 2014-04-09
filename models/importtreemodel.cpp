#include "importtreemodel.h"

// easy read convenience (:0, 1, 2)
enum Column { NAME, TYPE };

// fixed value
const int COLUMN_COUNT = 2;

ImportTreeModel::ImportTreeModel(QObject *parent) :
    QAbstractItemModel(parent), mRoot(0)
{
    mRoot = new ItemTemplate();
}

ImportTreeModel::~ImportTreeModel()
{
    delete mRoot; // recursive
}

Qt::ItemFlags ImportTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);

    if(index.isValid ()) {
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled;

        // users can change the name and toggle import
        if(index.column () == NAME) {
            theFlags |= Qt::ItemIsEditable;
        }
    }

    return theFlags;
}

QVariant ImportTreeModel::data(const QModelIndex &index, int role) const
{
    if(!mRoot || !index.isValid () || index.column () < 0 ||
            index.column () > COLUMN_COUNT) {
        return QVariant(); // invalid
    }


    if(ItemTemplate *item = itemFromIndex (index)) {

        // set data
        if(role == Qt::DisplayRole || role == Qt::EditRole) {
           switch(index.column ()) {
           case NAME:
               return item->name ();
           case TYPE:
               switch(item->importType ()) {
               case ItemTemplate::Single:
                   return tr("Single");
               case ItemTemplate::Group:
                   return tr("Group");
               case ItemTemplate::SpriteSheet:
                   return tr("SpriteSheet");
               case ItemTemplate::Decorator:
                   return tr("Decorator");
               default:
                   return QVariant();
               }
           default:
               Q_ASSERT(false); // no such column exists
           }
        }

        // left align all
        if(role == Qt::TextAlignmentRole) {
            return static_cast<int>(Qt::AlignVCenter |
                                    Qt::AlignLeft);
        }

        // item icon
        if(role == Qt::DecorationRole &&
                index.column () == NAME) {
            return item->icon ();
        }
    }

    return QVariant(); //invalid
}

bool ImportTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid () || index.column() > COLUMN_COUNT) {
        return false;
    }

    if(ItemTemplate *item = itemFromIndex (index)) {
        if (role == Qt::EditRole) {
            if(index.column () == NAME) {
                item->setName (value.toString());
                emit dataChanged (index, index);
            } else {
                return false;
            }
        }
    }

    emit layoutChanged();
    return true;
}

QVariant ImportTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section == NAME) {
            return tr("Name");
        } else if(section == TYPE) {
            return tr("Type");
        }
    }

    return QVariant(); // invalid
}

int ImportTreeModel::columnCount(const QModelIndex &parent
                                 __attribute__((unused))) const
{
    return COLUMN_COUNT;
}

int ImportTreeModel::rowCount(const QModelIndex &parent) const
{
    // we only allow children in the first coloumn (:0)
    if (parent.column() > 0 && parent.isValid()) {
        return 0;
    }

    ItemTemplate *parentItem = itemFromIndex (parent);

    /* if the item has a parent we return the index of the item
     * in the parents, children. if there is no parent, then it
     * must be root */
    return parentItem ? parentItem->childCount() : 0;
}

bool ImportTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(!mRoot) {
        mRoot = new ItemTemplate();

    }

    ItemTemplate *item = parent.isValid () ?
                itemFromIndex (parent) : mRoot;

    beginInsertRows (parent, row, row+count-1);

    for(int i=0; i < count; ++i) {
        (void) new ItemTemplate("", ItemTemplate::None, item);
    }

    endInsertRows ();

    return true;
}

bool ImportTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(!mRoot) {
        return false;
    }

    ItemTemplate *item = parent.isValid () ?
                itemFromIndex (parent) : mRoot;

    beginRemoveRows (parent, row, row+count-1);

    for(int i=0; i < count; ++i) {
        delete item->takeChild (row);
    }

    endRemoveRows ();

    return true;
}

QModelIndex ImportTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    /* if the parents column is not 0, then no child is allowed
     * and therefore no index is created */
    if(!mRoot || row < 0 || column < 0 || column >= COLUMN_COUNT
            || (parent.isValid() && parent.column() != 0)) {
        return QModelIndex(); // invalid
    }

    ItemTemplate *parentItem = itemFromIndex(parent);

    Q_ASSERT(parentItem); // must exist

    // retrieve parents row-th child
    if(ItemTemplate *item = parentItem->childAt (row)) {
        /* find the item and create QModelIndex with a pointer to
         * the child (: QModelIndex allows *void) */
        return createIndex(row, column, item);
    }

    return QModelIndex(); // invalid
}

QModelIndex ImportTreeModel::parent(const QModelIndex &child) const
{
    if(!child.isValid ()) {
        return QModelIndex(); // invalid
    }

    /* we need the index of the childs parent, in it's parent
     * (:grandparents) child array */
    if(ItemTemplate *item = itemFromIndex (child)) {
        if(ItemTemplate *parentItem = item->parent ()) {
            if(parentItem == mRoot) {
                return QModelIndex();
            }
            if(ItemTemplate *grandParentItem =
                    parentItem->parent ()) {
                int row = grandParentItem->rowOfChild (parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }

    return QModelIndex(); // invalid
}

ItemTemplate *ImportTreeModel::itemFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        if(ItemTemplate *item =
                static_cast<ItemTemplate *>(index.internalPointer ())) {
            return item;
        }
    }

    return mRoot;
}

void ImportTreeModel::addItemsFromUrls(const QList<QUrl> &urls)
{
    using namespace std;

    foreach(QUrl url, urls)
    {

        ItemTemplate *item = new ItemTemplate();

        item->setPath (QDir::toNativeSeparators(url.toLocalFile()));

        // remove extension (:.png)
        int index = url.fileName ().lastIndexOf (".");
        QString tmp = url.fileName ().remove (index,url.fileName ().count ());
        item->setName (tmp);
        item->setImage (QImage(item->path ()));

        item->setPixmap (new QGraphicsPixmapItem(
                    QPixmap::fromImage(item->image ())));

        QRect rect(item->image().rect ());

        item->scene ()->setSceneRect(rect);

        // set background
        QBrush brush;
        brush.setTextureImage(QImage(":/images/checkerboard"));
        QGraphicsRectItem *bg = new QGraphicsRectItem(rect);
        bg->setBrush (brush);
        bg->setPen (Qt::NoPen);

        item->scene ()->addItem (bg);
        item->scene()->addItem(item->pixmap ());

        item->setContour(mImgProc.findContours(item->path ()));
        item->setItemType (item->contour ().size() == 1 ?
                    ItemTemplate::Single : ItemTemplate::SpriteSheet);
        item->setConvex(mImgProc.findConvexes(item->path ()));
        item->setIcon (QIcon(QPixmap::fromImage (item->image ())));
        mRoot->addChild (item);
    }
}



