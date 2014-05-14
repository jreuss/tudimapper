#ifndef LEVELTREEMODEL_H
#define LEVELTREEMODEL_H
#include "models/abstracttreescenemodel.h"
#include "mainscene.h"

class LevelTreeModel : public AbstractTreeSceneModel
{
public:
    LevelTreeModel(int treeDepth);

    Qt::ItemFlags flags (const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData (const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
};

#endif // LEVELTREEMODEL_H
