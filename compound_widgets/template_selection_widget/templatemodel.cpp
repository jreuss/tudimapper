#include "templatemodel.h"

TemplateModel::TemplateModel(int treeDepth) : AbstractTreeModel(treeDepth)
{
    folderIcon = QIcon(QPixmap(":/icons/folder"));
}

Qt::ItemFlags TemplateModel::flags(const QModelIndex &index) const
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

QVariant TemplateModel::data(const QModelIndex &index, int role) const
{
    if(ItemTemplate *item = static_cast<ItemTemplate*>(itemFromIndex (index))) {

        // set data
        if(role == Qt::DisplayRole || role == Qt::EditRole) {
            if(index.column() == 0){
                return item->name();
            } else if (index.column() == 1) {
                return item->importType() == ItemTemplate::Folder ? "" : "template";
            }
        }

        if(role == Qt::DecorationRole &&
                index.column () == 0) {
           if(item->importType() == ItemTemplate::Folder){
                return folderIcon;
            } else {


             return QPixmap( *item->pixmap());
            }

        }
        if(role == Qt::TextAlignmentRole) {
            return static_cast<int>(Qt::AlignVCenter |
                                    Qt::AlignLeft);
        }

    }
    return QVariant();
//        if(index.isValid() && role == Qt::DisplayRole) {

//            ItemTemplate *item = static_cast<ItemTemplate*>(itemFromIndex (index));
//            if(index.column() == 0){
//                return item->name();
//            }

//        }

    //}

   // return QVariant();
}

bool TemplateModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if(index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        ItemTemplate *item = static_cast<ItemTemplate*>(itemFromIndex (index));
        item->setName(value.toString());
        QModelIndex topLeft = index;
        QModelIndex bottomRight = index;
        emit dataChanged(topLeft, bottomRight);
        return true;
    }
    return false;
}

QVariant TemplateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section ==0) {
            return tr("Name");
        }
    }

    return QVariant(); // invalid
}

