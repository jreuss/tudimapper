#ifndef IMPORTTREEMODEL_H
#define IMPORTTREEMODEL_H

#include "stable_pch.h"
#include "items/itemtemplate.h"
#include "items/abstracttreeitem.h"
#include "abstracttreemodel.h"
#include "utility/imgproc.h"

class ImportTreeModel : public AbstractTreeModel
{
    Q_OBJECT
public:
    explicit ImportTreeModel(const int &treeDepth = 2);

    /* =======================================
     * region: virtual overrides
     * =====================================*/

    Qt::ItemFlags flags (const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData (const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData (int section, Qt::Orientation orientation, int role) const;

    void addItemsFromUrls(const QList<QUrl> &urls);


signals:
    void onRemoveSplitParent();
    void onExpandSplit(ItemTemplate*);
    void onSelectFirstUrl(ItemTemplate*);

public slots:
   void handleSplit(ItemTemplate*, bool, float);
   void handleSplitAndAddToScene(ItemTemplate*, bool, float);

private:

    ImgProc mImgProc;
};

#endif // IMPORTTREEMODEL_H
