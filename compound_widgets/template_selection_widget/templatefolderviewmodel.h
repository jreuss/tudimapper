#ifndef TemplateFolderViewModel_H
#define TemplateFolderViewModel_H

#include "models/abstracttreemodel.h"
#include "folderitem.h"
#include <QIcon>
#include <QStringList>
#include <QMimeData>

class TemplateFolderViewModel : public AbstractTreeModel
{
    Q_OBJECT
public:
    TemplateFolderViewModel(const int &treeDepth, QObject *parent = 0);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const ;
    bool setData (const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;

    QIcon getFolderIcon() const;
    void setFolderIcon(const QIcon &value);

    Qt::DropActions supportedDropActions() const;
    Qt::DropActions supportedDragActions() const;

    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QStringList mimeTypes() const;

signals:
    void onItemDropped(QStringList, AbstractTreeItem*);
private:
    QIcon mFolderIcon;
};

#endif // TemplateFolderViewModel_H
