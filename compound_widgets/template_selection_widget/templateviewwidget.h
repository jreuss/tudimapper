#ifndef TEMPLATEVIEWWIDGET_H
#define TEMPLATEVIEWWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QLayout>
#include <QIcon>
#include <QListWidget>
#include <QTreeView>
#include <QLayout>
#include <QSplitter>
#include <QDebug>
#include <QHash>
#include <QMenu>
#include <QTimer>

#include "thumbnailgridview.h"
#include "delegates/templatedelegate.h"
#include "templatefolderviewmodel.h"
#include "items/itemtemplate.h"
#include "templatethumbmodel.h"
#include "utility/globals.h"

namespace Ui {
class TemplateViewWidget;
}

class TemplateViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateViewWidget(QWidget *parent = 0);
    void setupConnections();
    void createActions();
    ~TemplateViewWidget();
    void addTemplates(ItemTemplate *templateRoot);
    void moveFolder(FolderItem* src, FolderItem* dst);

public slots:
    void handleFolderSelectionChanged(QModelIndex);
    void handleOnTemplateDropped(QStringList, AbstractTreeItem*);
    void handleOnFolderDropped(QStringList, AbstractTreeItem*);

    // folderview contextmenu handlers
    void handleCustomContextMenu(const QPoint &);
    void handleAddFolder();
    void handleRemoveFolder();
    void handleCopyFolder();
    void handlePasteFolder();

private:
    QModelIndex currentFolderIndex;
    FolderItem *currentFolder;
    QMenu folderContextMenu;
    TemplateFolderViewModel *mFolderModel;
    QTreeView *mTreeView;
    Ui::TemplateViewWidget *ui;
    TemplateThumbModel *mTemplateModel;
    ThumbnailGridView *mTiledListView;
    QHBoxLayout *mLayout;
    QSplitter *splitter;
};

#endif // TEMPLATEVIEWWIDGET_H
