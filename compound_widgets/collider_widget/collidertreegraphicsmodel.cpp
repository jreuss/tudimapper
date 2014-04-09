#include "collidertreegraphicsmodel.h"
#include "colliderwidget.h"

ColliderTreeGraphicsModel::ColliderTreeGraphicsModel(const int &treeDepth) : AbstractTreeGraphicsModel(treeDepth)
{
}

QVariant ColliderTreeGraphicsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section == 0) {
            return tr("Name");
        } else if (section == 1) {
            return tr("Type");
        }
        else if(section == 2){
            return tr("Trigger");
        }
    }

    return QVariant(); // invalid
}

Qt::ItemFlags ColliderTreeGraphicsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);

    if(index.isValid ()) {
        theFlags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if(index.column() == 0 || index.column() == 2) {
            theFlags |= Qt::ItemIsEditable;
        }
    }
    return theFlags;
}

QVariant ColliderTreeGraphicsModel::data(const QModelIndex &index, int role) const
{
    if(!mRoot || !index.isValid () || index.column () < 0 ||
            index.column () > TREE_DEPTH) {
        return QVariant(); // invalid
    }


    if(Collider *item = static_cast<Collider*>(itemFromIndex (index))) {

        // set data
        if(role == Qt::DisplayRole || role == Qt::EditRole) {
            if( index.column()==0){
                return item->getName();
            }
            else if(index.column()==1){
                if(item->type() == ColliderWidget::MeshType){
                    return "Polygon";
                } else if(item->type() == ColliderWidget::BoxType){
                    return "Box";
                } else if(item->type() == ColliderWidget::CircleType){
                return "Circle";
            }
            }
            else if(index.column()==2)
                return item->getIsTrigger();
        }
        // left align all
        if(role == Qt::TextAlignmentRole) {
            return static_cast<int>(Qt::AlignVCenter |
                                    Qt::AlignLeft);
        }
    }


    return QVariant(); //invalid
}

bool ColliderTreeGraphicsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid () || index.column() > TREE_DEPTH) {
        return false;
    }

    if(Collider *item = static_cast<Collider*>(itemFromIndex (index))) {
          if (role == Qt::EditRole) {
              if(index.column () == 0){
                  item->setName(value.toString());
                  emit dataChanged (index, index);
              }
              else if(index.column () == 2) {
                  item->setIsTrigger(value.toBool());
                  emit dataChanged (index, index);
              } else {
                  return false;
              }
          }
      }
    emit layoutChanged();
    return true;
}




