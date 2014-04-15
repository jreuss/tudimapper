#ifndef TEMPLATETHUMBMODEL_H
#define TEMPLATETHUMBMODEL_H

#include <QMimeData>
#include <QDataStream>

#include "models/abstracttreemodel.h"
#include "items/itemtemplate.h"
#include "folderitem.h"

class TemplateThumbModel : public AbstractTreeModel
{
    Q_OBJECT
public:
    enum SECTIONS {
        NAME
    };
    TemplateThumbModel(const int &treeDepth);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const ;
    bool setData (const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const ;

    Qt::DropActions supportedDropActions() const;
    Qt::DropActions supportedDragActions() const;

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

signals:
   void onItemDropped(QStringList, AbstractTreeItem*);

};

#endif // TEMPLATETHUMBMODEL_H
