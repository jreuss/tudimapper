#ifndef FOLDERPROXY_H
#define FOLDERPROXY_H

#include "stable_pch.h"

class FolderProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FolderProxy(QObject *parent = 0);

signals:

public slots:


    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // FOLDERPROXY_H
