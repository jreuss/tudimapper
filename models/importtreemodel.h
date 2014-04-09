#ifndef IMPORTTREEMODEL_H
#define IMPORTTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QDebug>
#include <QList>
#include <QUrl>
#include <QDir>
#include <QImage>

#include "items/itemtemplate.h"
#include "utility/imgproc.h"

class ImportTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ImportTreeModel(QObject *parent = 0);

    ~ImportTreeModel();

    /* =======================================
     * region: virtual overrides
     * =====================================*/

    Qt::ItemFlags flags (const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData (const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;

    // fixed header
    bool setHeaderData (int section __attribute__((unused)),
                        Qt::Orientation orientation __attribute__((unused)),
                        const QVariant &value __attribute__((unused)),
                        int role __attribute__((unused)))
    { return false;}

    int columnCount (const QModelIndex &parent) const;

    int rowCount(const QModelIndex &parent) const;

    bool insertRows(int row, int count, const QModelIndex &parent);

    bool removeRows (int row, int count, const QModelIndex &parent);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    QModelIndex parent(const QModelIndex &child) const;

    /* =======================================
     * region: utility
     * =====================================*/

    ItemTemplate *itemFromIndex(const QModelIndex &index) const;

    void addItemsFromUrls(const QList<QUrl> &urls);


signals:

public slots:

private:

    ItemTemplate *mRoot;

    ImgProc mImgProc;
};

#endif // IMPORTTREEMODEL_H
