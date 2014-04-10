#ifndef TEMPLATETHUMBMODEL_H
#define TEMPLATETHUMBMODEL_H

#include "models/abstracttreemodel.h"
#include "items/itemtemplate.h"

class TemplateThumbModel : public AbstractTreeModel
{
public:
    TemplateThumbModel(const int &treeDepth, QObject *parent = 0);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const ;
    bool setData (const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const ;
};

#endif // TEMPLATETHUMBMODEL_H
