#ifndef ELEMENTTREEMODEL_H
#define ELEMENTTREEMODEL_H

#include "models/abstracttreepixmapmodel.h"
#include "items/itemelement.h"

class ElementTreeModel : public AbstractTreePixmapModel
{
public:
    ElementTreeModel(int treeDepth);

    Qt::ItemFlags flags (const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData (const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;

};

#endif // ELEMENTTREEMODEL_H
