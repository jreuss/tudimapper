#include "templatefolderviewmodel.h"

enum SECTIONS {
    ICON, NAME
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
            return tr("Name");
        }
    }

    return QVariant(); // invalid
}

Qt::ItemFlags TemplateFolderViewModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);

    if(index.isValid ()) {
        theFlags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
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
                index.column () == ICON) {
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

QIcon TemplateFolderViewModel::getFolderIcon() const
{
    return mFolderIcon;
}

void TemplateFolderViewModel::setFolderIcon(const QIcon &value)
{
    mFolderIcon = value;
}

