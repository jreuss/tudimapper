#include "elementtreemodel.h"


ElementTreeModel::ElementTreeModel(int treeDepth) : AbstractTreePixmapModel(treeDepth)
{
}

Qt::ItemFlags ElementTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);

    if(index.isValid ()) {
        // users can change the name and toggle import
        if(index.column () == 0) {
            theFlags |= Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
    }

    return theFlags;
}

QVariant ElementTreeModel::data(const QModelIndex &index, int role) const
{
    if(ItemElement *item = static_cast<ItemElement*>(itemFromIndex (index))) {

        // set data
        if(role == Qt::DisplayRole || role == Qt::EditRole) {
            if(index.column() == 0){
                return item->getName();
            }
        }

        if(role == Qt::TextAlignmentRole) {
            return static_cast<int>(Qt::AlignVCenter |
                                    Qt::AlignLeft);
        }

    }
    return QVariant();
}

bool ElementTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if(index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        ItemElement *item = static_cast<ItemElement*>(itemFromIndex (index));
        item->setName(value.toString());
        QModelIndex topLeft = index;
        QModelIndex bottomRight = index;
        emit dataChanged(topLeft, bottomRight);
        return true;
    }
    return false;
}

QVariant ElementTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section ==0) {
            return tr("Element name");
        }
    }

    return QVariant(); // invalid
}

