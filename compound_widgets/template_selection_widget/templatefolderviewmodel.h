#ifndef TEMPLATEFOLDERVIEWMODEL_H
#define TEMPLATEFOLDERVIEWMODEL_H

#include "models/abstracttreemodel.h"
#include "folderitem.h"
#include <QIcon>

class templatefolderviewmodel : public AbstractTreeModel
{
public:
    templatefolderviewmodel(const int &treeDepth, QObject *parent = 0);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const ;
    bool setData (const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const ;

    QIcon getFolderIcon() const;
    void setFolderIcon(const QIcon &value);

private:
    QIcon mFolderIcon;
};

#endif // TEMPLATEFOLDERVIEWMODEL_H
