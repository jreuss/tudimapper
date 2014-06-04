#ifndef TEMPLATEMODEL_H
#define TEMPLATEMODEL_H

#include "models/abstracttreemodel.h"
#include "items/itemtemplate.h"

class TemplateModel : public AbstractTreeModel
{
public:
    TemplateModel(int treeDepth);

    Qt::ItemFlags flags (const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData (const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;

    QPixmap folderIcon;

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;

};

#endif // TEMPLATEMODEL_H
