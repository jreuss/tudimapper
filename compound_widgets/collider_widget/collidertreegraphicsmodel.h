#ifndef COLLIDERTREEGRAPHICSMODEL_H
#define COLLIDERTREEGRAPHICSMODEL_H

#include "colliders/collider.h"
#include "models/abstracttreegraphicsmodel.h"

class ColliderTreeGraphicsModel : public AbstractTreeGraphicsModel
{
public:
    ColliderTreeGraphicsModel(const int &treeDepth);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const ;
    bool setData (const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const ;

};

#endif // COLLIDERTREEGRAPHICSMODEL_H
