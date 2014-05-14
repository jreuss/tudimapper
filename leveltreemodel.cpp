#include "leveltreemodel.h"

LevelTreeModel::LevelTreeModel(int treeDepth) : AbstractTreeSceneModel(treeDepth)
{
}

Qt::ItemFlags LevelTreeModel::flags(const QModelIndex &index) const
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

QVariant LevelTreeModel::data(const QModelIndex &index, int role) const
{
    if(MainScene *scene = static_cast<MainScene*>(itemFromIndex (index))) {

        // set data
        if(role == Qt::DisplayRole || role == Qt::EditRole) {
            if(index.column() == 0){
                return scene->getName();
            }
        }

        if(role == Qt::TextAlignmentRole) {
            return static_cast<int>(Qt::AlignVCenter |
                                    Qt::AlignLeft);
        }

    }
    return QVariant();
}

bool LevelTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if(index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        MainScene *scene = static_cast<MainScene*>(itemFromIndex (index));
        scene->setName(value.toString());
        QModelIndex topLeft = index;
        QModelIndex bottomRight = index;
        emit dataChanged(topLeft, bottomRight);
        return true;
    }
    return false;
}

QVariant LevelTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section ==0) {
            return tr("Level name");
        }
    }

    return QVariant(); // invalid
}


