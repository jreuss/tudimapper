#include "folderproxy.h"

FolderProxy::FolderProxy(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}


bool FolderProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex nodeIndex = sourceModel()->index(source_row,1,source_parent);
    QString type = sourceModel()->data(nodeIndex).toString();


    return type == "template" ? false : true;
}
