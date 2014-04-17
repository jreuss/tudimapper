#include "templatefolderviewmodel.h"

enum SECTIONS {
    NAME
};

TemplateFolderViewModel::TemplateFolderViewModel(const int &treeDepth, QObject *parent) :
    AbstractTreeModel(treeDepth, parent)
{
    mFolderIcon = QIcon(":/icons/folder");
}

QVariant TemplateFolderViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case NAME:
            return tr("Assets");
        }
    }

    return QVariant(); // invalid
}

Qt::ItemFlags TemplateFolderViewModel::flags(const QModelIndex &index) const
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

bool TemplateFolderViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid () || index.column() > TREE_DEPTH) {
        return false;
    }

    if(FolderItem *item = static_cast<FolderItem *>(itemFromIndex (index))) {
        if (role == Qt::EditRole) {
            switch(index.column()) {
            case NAME:
                item->setName(value.toString());
                emit dataChanged(index,index, QVector<int>() << Qt::DisplayRole <<  Qt::EditRole);
                emit layoutChanged();
                break;
            }
        } else {
            return false;
        }
    }
    return true;
}

QVariant TemplateFolderViewModel::data(const QModelIndex &index, int role) const
{
    if(!mRoot || !index.isValid () || index.column () < 0 ||
            index.column () > TREE_DEPTH) {
        return QVariant(); // invalid
    }

    if(FolderItem *item = static_cast<FolderItem *>(itemFromIndex (index))) {

        // set data
        if(role == Qt::DisplayRole || role == Qt::EditRole) {
            switch(index.column()) {
            case NAME:
                return item->getName();
            }
        }

        // item icon
        if(role == Qt::DecorationRole &&
                index.column () == NAME) {
            return mFolderIcon;
        }

        // left align all
        if(role == Qt::TextAlignmentRole) {
            return static_cast<int>(Qt::AlignVCenter |
                                    Qt::AlignLeft);
        }
    }
    return QVariant(); //invalid
}

Qt::DropActions TemplateFolderViewModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions TemplateFolderViewModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QIcon TemplateFolderViewModel::getFolderIcon() const
{
    return mFolderIcon;
}

void TemplateFolderViewModel::setFolderIcon(const QIcon &value)
{
    mFolderIcon = value;
}

QStringList TemplateFolderViewModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData *TemplateFolderViewModel::mimeData(const QModelIndexList &indexes) const
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

bool TemplateFolderViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);

    AbstractTreeItem *itmParent = itemFromIndex(parent);

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList dragSelection;

    while(!stream.atEnd()) {
        QString txt;
        stream >> txt;
        dragSelection << txt;
    }

    emit onItemDropped(dragSelection, itmParent);
    return false;
}


