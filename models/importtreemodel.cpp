#include "importtreemodel.h"

// easy read convenience (:0, 1, 2)
enum Column { NAME, TYPE };

// fixed value
const int COLUMN_COUNT = 2;

ImportTreeModel::ImportTreeModel(const int &treeDepth) :
    AbstractTreeModel(treeDepth)
{
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


    if(ItemTemplate *item = static_cast<ItemTemplate*>(itemFromIndex (index))) {

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
                case ItemTemplate::Split:
                    return tr("Split");
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

        if ( index.isValid() && role == Qt::ForegroundRole )
            {
                if ( item->importType() == ItemTemplate::Group)
                {
                    return QVariant( QColor( Qt::red ) );
                }
                return QVariant( QColor( Qt::black ) );
            }
    }

    return QVariant(); //invalid
}

bool ImportTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid () || index.column() > COLUMN_COUNT) {
        return false;
    }

    if(ItemTemplate *item = static_cast<ItemTemplate*>(itemFromIndex (index))) {
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


void ImportTreeModel::addItemsFromUrls(const QList<QUrl> &urls)
{
    using namespace std;
    bool first = true;
    foreach(QUrl url, urls)
    {

        ItemTemplate *item = new ItemTemplate();

        item->setPath (QDir::toNativeSeparators(url.toLocalFile()));

        // remove extension (:.png)
        int index = url.fileName ().lastIndexOf (".");
        QString tmp = url.fileName ().remove (index,url.fileName ().count ());
        item->setName (tmp);
        item->setImage (QImage(item->path ()));

        item->setPixmap (new QPixmap(
                             QPixmap::fromImage(item->image ())));

        item->setPixmapItem(new QGraphicsPixmapItem(*item->pixmap()));


        item->calculateSceneRect();


        item->scene()->addItem(item->getPixmapItem());

        item->setContour(mImgProc.findContours(item->path ()));
        item->setItemType (item->contour ().size() == 1 ?
                               ItemTemplate::Single : ItemTemplate::Group);
        item->setConvex(mImgProc.findConvexes(item->path ()));
        item->setIcon (QIcon(*item->pixmap()));
        //AbstractTreeModel::insertItem(mRoot->mChildren.count());
        this->insertItem(getRoot()->getChildren().count(), getRoot(), item);
        if(first){
            emit onSelectFirstUrl(item);
            first=false;
        }
    }
}

void ImportTreeModel::handleSplit(ItemTemplate *item, bool removeDuplicates, float shapeTresh)
{
    QList<ItemTemplate*> templates;
    if(removeDuplicates){
        QList<QList<unsigned> > colorMaches = mImgProc.get_colorMatches(0.999,item->path(),item->contour());
        templates = mImgProc.splitImageAndRemoveDuplicates(item->contour(),item->path(),shapeTresh,colorMaches);
    }else {
        templates = mImgProc.splitImage(item->contour(),item->path());
    }
    foreach(ItemTemplate *itm, templates){
        this->insertItem(getRoot()->getChildren().count(),getRoot(), itm);
    }
    emit onRemoveSplitParent();
}

void ImportTreeModel::handleSplitAndAddToScene(ItemTemplate *item, bool removeDuplicates, float shapeTres)
{
    item->setItemType(ItemTemplate::Split);
    QList<ItemTemplate*> templates;
    if(removeDuplicates){
        QList<QList<unsigned> > colorMaches = mImgProc.get_colorMatches(0.999,item->path(),item->contour());
        templates = mImgProc.splitImageAndRemoveDuplicatesAddToScene(item->contour(),item->path(),shapeTres,colorMaches,item->getSplitScene());
        foreach(ItemTemplate *itm, templates){
            this->insertItem(item->mChildren.count(),item, itm);
        }
    } else {


        templates = mImgProc.splitAndAddToScene(item->contour(),item->path(),item->getSplitScene());

        foreach(ItemTemplate *itm, templates){
            item->addChild(itm);
        }
    }

    //item->setSplitScene(thePair.second);

    layoutChanged();
    emit onExpandSplit(item);
}




