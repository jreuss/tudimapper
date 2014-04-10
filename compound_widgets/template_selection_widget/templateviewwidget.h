#ifndef TEMPLATEVIEWWIDGET_H
#define TEMPLATEVIEWWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QLayout>
#include <QIcon>
#include <QListWidget>
#include <QTreeView>
#include <QDebug>

#include "thumbnailgridview.h"
#include "delegates/templatedelegate.h"
#include "templatefolderviewmodel.h"
#include "items/itemtemplate.h"
#include "templatethumbmodel.h"

namespace Ui {
class TemplateViewWidget;
}

class TemplateViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TemplateViewWidget(QWidget *parent = 0);
    void setupConnections();
    ~TemplateViewWidget();


public slots:
    void handleFolderSelectionChanged(QModelIndex);

private:
    TemplateFolderViewModel *mFolderModel;
    QTreeView *mTreeView;
    Ui::TemplateViewWidget *ui;
    TemplateThumbModel *mTemplateModel;
    ThumbnailGridView *mTiledListView;
    QHBoxLayout *mLayout;
};

#endif // TEMPLATEVIEWWIDGET_H
