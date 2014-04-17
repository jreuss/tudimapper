#include "templatethumbmodel.h"

TemplateThumbModel::TemplateThumbModel(const int &treeDepth) :
                                       AbstractTreeModel(treeDepth)
{

}

QVariant TemplateThumbModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    Q_UNUSED(section);

    return QVariant(); // invalid
}

Qt::ItemFlags TemplateThumbModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);

    if(index.isValid ()) {
        theFlags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
        if(index.column() == NAME) {
            theFlags |= Qt::ItemIsEditable;
        }
    }
    return theFlags;
}

bool TemplateThumbModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid () || index.column() > TREE_DEPTH) {
        return false;
    }

    AbstractTreeItem *item = itemFromIndex (index);
    if(item->getItemType() == AbstractTreeItem::TemplateType){

        if(ItemTemplate *templateItem = static_cast<ItemTemplate *>(item)) {
            if (role == Qt::EditRole) {
                switch(index.column()) {
                case NAME:
                    templateItem->setName(value.toString());
                    emit dataChanged(index,index, QVector<int>() << Qt::DisplayRole <<  Qt::EditRole);
                    emit layoutChanged();
                    break;
                }
            } else {
                return false;
            }
        }
    } else if(item->getItemType() == AbstractTreeItem::FolderType){

        if(FolderItem *folderItem = static_cast<FolderItem *>(item)) {
            if (role == Qt::EditRole) {
                switch(index.column()) {
                case NAME:
                    folderItem->setName(value.toString());
                    emit dataChanged(index,index, QVector<int>() << Qt::DisplayRole <<  Qt::EditRole);
                    emit layoutChanged();
                    break;
                }
            } else {
                return false;
            }
        }
    }
    return true;
}

QVariant TemplateThumbModel::data(const QModelIndex &index, int role) const
{
    if(!mRoot || !index.isValid () || index.column () < 0 ||
            index.column () > TREE_DEPTH) {
        return QVariant(); // invalid
    }

    AbstractTreeItem *item = itemFromIndex (index);
    if(item->getItemType() == AbstractTreeItem::TemplateType){
        if(ItemTemplate *templateItem = static_cast<ItemTemplate *>(item)) {

            // set data
            if(role == Qt::DisplayRole || role == Qt::EditRole) {
                switch(index.column()) {
                case NAME:
                    return templateItem->name();
                }
            }

            // item icon
            if(role == Qt::DecorationRole &&
                    index.column () == NAME) {
                return QPixmap(*templateItem->pixmap());
            }

            // left align all
            if(role == Qt::TextAlignmentRole) {
                return static_cast<int>(Qt::AlignVCenter |
                                        Qt::AlignLeft);
            }
        }
    } else if(item->getItemType() == AbstractTreeItem::FolderType){
        if(FolderItem *folderItem = static_cast<FolderItem *>(item)) {

            // set data
            if(role == Qt::DisplayRole || role == Qt::EditRole) {
                switch(index.column()) {
                case NAME:
                    return folderItem->getName();
                }
            }

            // item icon
            if(role == Qt::DecorationRole &&
                    index.column () == NAME) {
                return QPixmap(":/icons/folder");
            }

            // left align all
            if(role == Qt::TextAlignmentRole) {
                return static_cast<int>(Qt::AlignVCenter |
                                        Qt::AlignLeft);
            }
        }
    }
    return QVariant(); //invalid
}

Qt::DropActions TemplateThumbModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions TemplateThumbModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList TemplateThumbModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

void AbstractTreeModel::reset()
{
    beginResetModel();
    mRoot = NULL;
    endResetModel();
}

QMimeData *TemplateThumbModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach(QModelIndex index, indexes) {
        if(index.isValid()) {
            AbstractTreeItem* itm = itemFromIndex(index);
            stream << itm->getID();
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

bool TemplateThumbModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);

    AbstractTreeItem *itmParent = itemFromIndex(parent);

    if(itmParent->getItemType() == AbstractTreeItem::FolderType) {

        QByteArray encodedData = data->data("application/vnd.text.list");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        QStringList dragSelection;

        while(!stream.atEnd()) {
            QString txt;
            stream >> txt;
            dragSelection << txt;
        }

        emit onItemDropped(dragSelection, itmParent);
    }

    return false;
}
