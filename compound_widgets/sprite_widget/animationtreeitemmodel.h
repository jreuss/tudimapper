#ifndef ANIMATIONTREEITEMMODEL_H
#define ANIMATIONTREEITEMMODEL_H

#include "models/abstracttreemodel.h"
#include "animationssequence.h"

class AnimationTreeItemModel : public AbstractTreeModel
{
public:
    AnimationTreeItemModel(const int &treeDepth);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const ;
    bool setData (const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const ;
};

#endif // ANIMATIONTREEITEMMODEL_H
