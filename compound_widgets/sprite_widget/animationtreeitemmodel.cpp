#include "animationtreeitemmodel.h"

enum SECTIONS {
    NAME, START_ROW, START_COLUMN,
    END_ROW, END_COLUMN, ANIMATION_SPEED
};

AnimationTreeItemModel::AnimationTreeItemModel(const int &treeDepth) :
    AbstractTreeModel(treeDepth)
{
}

QVariant AnimationTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case NAME:
            return tr("Name");
        case START_ROW:
            return tr("Start row");
        case START_COLUMN:
            return tr("Start column");
        case END_ROW:
            return tr("End row");
        case END_COLUMN:
            return tr("End column");
        case ANIMATION_SPEED:
            return tr("Animation speed");
        }
    }

    return QVariant(); // invalid
}

Qt::ItemFlags AnimationTreeItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);

    if(index.isValid ()) {
        theFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
    return theFlags;
}

QVariant AnimationTreeItemModel::data(const QModelIndex &index, int role) const
{
    if(!mRoot || !index.isValid () || index.column () < 0 ||
            index.column () > TREE_DEPTH) {
        return QVariant(); // invalid
    }

    if(AnimationsSequence *item = static_cast<AnimationsSequence *>(itemFromIndex (index))) {

        // set data
        if(role == Qt::DisplayRole || role == Qt::EditRole) {
            switch(index.column()) {
            case NAME:
                return item->getName();
            case START_ROW:
                return item->getStartRow();
            case START_COLUMN:
                return item->getStartColumn();
            case END_ROW:
                return item->getEndRow();
            case END_COLUMN:
                return item->getEndColumn();
            case ANIMATION_SPEED:
                return item->getAnimationSpeed();
            }
        }

        // left align all
        if(role == Qt::TextAlignmentRole) {
            return static_cast<int>(Qt::AlignVCenter |
                                    Qt::AlignLeft);
        }
    }
    return QVariant(); //invalid
}

bool AnimationTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid () || index.column() > TREE_DEPTH) {
        return false;
    }

    if(AnimationsSequence *item = static_cast<AnimationsSequence *>(itemFromIndex (index))) {
        if (role == Qt::EditRole) {
            switch(index.column()) {
            case NAME:
                item->setName(value.toString());
                break;
            case START_ROW:
                item->setStartRow(value.toInt());
                break;
            case START_COLUMN:
                item->setStartColumn(value.toInt());
                break;
            case END_ROW:
                item->setEndRow(value.toInt());
                break;
            case END_COLUMN:
                item->setEndColumn(value.toInt());
                break;
            case ANIMATION_SPEED:
                item->setAnimationSpeed(value.toInt());
                break;
            }
        } else {
            return false;
        }
    }
    emit dataChanged(index,index, QVector<int>() << Qt::DisplayRole <<  Qt::EditRole);
    emit layoutChanged();
    return true;
}



