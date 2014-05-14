#ifndef TEMPLATEFOLDERWIDGET_H
#define TEMPLATEFOLDERWIDGET_H

#include <QSplitter>

#include "stable_pch.h"
#include "thumbnailgridview.h"
#include "folderproxy.h"
#include "templatemodel.h"
#include "delegates/templatedelegate.h"


namespace Ui {
class TemplateFolderWidget;
}

class TemplateFolderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TemplateFolderWidget(QWidget *parent = 0);
    ~TemplateFolderWidget();

public slots:
    void addTemplates(ItemTemplate*);
    void handleAddFolderAction();
    void handleAddTemplateAction();
    void handleRemoveAction();
    void handleContextMenuRequested(QPoint);
    void handleFolderSelectionChanged(QModelIndex);
    void handleRequestedTemplates(QPointF);

signals:
    void mapProxySelection(QModelIndex);
    void onSendRequestedTemplates(QPointF, QList<ItemTemplate*>);

private:
    Ui::TemplateFolderWidget *ui;
    TemplateModel *model;
    FolderProxy *folderProxy;
    QTreeView *folderView;
    ThumbnailGridView *thumbView;
    QSplitter *splitter;
    QHBoxLayout *hLayout;
    QMenu modelContextMenu;
    QModelIndex currentModelIndex;
    void createActions();
};


#endif // TEMPLATEFOLDERWIDGET_H
