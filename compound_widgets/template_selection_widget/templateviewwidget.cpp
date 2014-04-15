#include "templateviewwidget.h"
#include "ui_templateviewwidget.h"

#include <QStandardItemModel>
#include "utility/globals.h"

TemplateViewWidget::TemplateViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemplateViewWidget)
{
    ui->setupUi(this);

    // create folder model
    mFolderModel = new TemplateFolderViewModel(1);
    mTreeView = new QTreeView();

    mTreeView->setModel(mFolderModel);
    mTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    mTemplateModel = new TemplateThumbModel(2);

    splitter = new QSplitter(Qt::Horizontal);

    mTiledListView = new ThumbnailGridView();
    mTiledListView->setItemDelegate(new TemplateDelegate());
    mTiledListView->setModel(mTemplateModel);

    mLayout = new QHBoxLayout();
    splitter->addWidget(mTreeView);
    splitter->addWidget(mTiledListView);
    mLayout->addWidget(splitter);

    setLayout(mLayout);
    setAcceptDrops(true);

    setupConnections();
    createActions();

    currentFolder = NULL;
    currentFolderIndex = QModelIndex();
}

void TemplateViewWidget::setupConnections()
{
    connect(mTreeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(handleFolderSelectionChanged(QModelIndex)));

    connect(mTemplateModel, SIGNAL(onItemDropped(QStringList, AbstractTreeItem*)),
            this, SLOT(handleOnItemDropped(QStringList, AbstractTreeItem*)));

    connect(mTreeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(handleCustomContextMenu(const QPoint &)));
}

void TemplateViewWidget::createActions()
{
    QAction *newFolderAction = new QAction(tr("New"), this);
    connect(newFolderAction, SIGNAL(triggered()), this, SLOT(handleAddFolder()));
    folderContextMenu.addAction(newFolderAction);

    QAction *copyFolderAction = new QAction(tr("Copy"), this);
    connect(copyFolderAction, SIGNAL(triggered()), this, SLOT(handleCopyFolder()));
    folderContextMenu.addAction(copyFolderAction);

    QAction *pasteFolderAction = new QAction(tr("Paste"), this);
    connect(pasteFolderAction, SIGNAL(triggered()), this, SLOT(handlePasteFolder()));
    folderContextMenu.addAction(pasteFolderAction);

    QAction *removeFolderAction = new QAction(tr("Remove"), this);
    connect(removeFolderAction, SIGNAL(triggered()), this, SLOT(handleRemoveFolder()));
    folderContextMenu.addAction(removeFolderAction);
}

TemplateViewWidget::~TemplateViewWidget()
{
    delete mTemplateModel;
    delete mFolderModel;
    delete mTiledListView;
    delete mTreeView;
    delete mLayout;
    delete ui;
}

void TemplateViewWidget::addTemplates(ItemTemplate *templateRoot)
{
    // create new top-most folder
    FolderItem *itm = new FolderItem("Imported");

    // insert the folder into the folder hash
    globs::folderHash.insert(itm->getID(), itm);

    // add templates to the folder
    itm->getFolderRoot()->addChildren(templateRoot->getChildren());

    // add all the newly added items to the template hash
    foreach(AbstractTreeItem* i, itm->getFolderRoot()->getChildren()) {
        i->setFolderID(itm->getID());
        globs::templateHash.insert(i->getID(), i);
    }

    // add the folder to the model
    mFolderModel->getRoot()->addChild(itm);

    /*==================================================
     * test remove when done
     * ===============================================*/

    // create a child folder
    FolderItem *child = new FolderItem("Sub folder");
    child->setFolderID(itm->getID());
    // (stupid..) add the child to the folders contained items,
    // making it visible in the thumbview
    itm->getFolderRoot()->addChild(child);
    // also add the child folder to the top-most folder, for
    // it to be displayed in the folderview
    itm->addChild(child);

    // register the child with the folder hash
    globs::folderHash.insert(child->getID(), child);

    //child->setParent(itm); redundant??

    // update the folder view
    mFolderModel->layoutChanged();
}

void TemplateViewWidget::dropEvent(QDropEvent *event)
{
    QByteArray encodedData = event->mimeData()->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString text;

    while(!stream.atEnd()) {
        QString text;
        stream >> text;
    }
}

void TemplateViewWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void TemplateViewWidget::handleFolderSelectionChanged(QModelIndex idx)
{
    FolderItem *tmp = static_cast<FolderItem*>(mFolderModel->itemFromIndex(idx));
    mTemplateModel->setRoot(tmp->getFolderRoot());
    mTiledListView->forceUpdate();
}

void TemplateViewWidget::handleOnItemDropped(QStringList itemIDs, AbstractTreeItem* parent)
{
    FolderItem *targetFolder = static_cast<FolderItem*>(parent);

    foreach(QString s, itemIDs) {
        AbstractTreeItem *itm = globs::templateHash.value(s);
        if(itm->getItemType() == AbstractTreeItem::TemplateType) {
            targetFolder->switchItemToThisFolder(itm);
        }
    }
}

void TemplateViewWidget::handleCustomContextMenu(const QPoint &pos)
{
    // get the clicked index
    QModelIndex index = mTreeView->indexAt(pos);

    currentFolderIndex = index;
    folderContextMenu.popup(mTreeView->viewport()->mapToGlobal(pos));
}

void TemplateViewWidget::handleAddFolder()
{
    // create a new subfolder
    FolderItem *f = new FolderItem("Folder" + QString::number(globs::folderHash.count()));

    // insert the folder into the folder hash
    globs::folderHash.insert(f->getID(), f);

    // this means we have clicked on top of an
    // existing folder (:new folder added as child)
    if(currentFolderIndex.isValid()) {
        // get the clicked folder
        FolderItem *parent =
                static_cast<FolderItem*>(mFolderModel->itemFromIndex(currentFolderIndex));

        // add a new sub folder
        f->setFolderID(parent->getID()); // set its parent folder id
        parent->getFolderRoot()->addChild(f); // add to thumb view
        parent->addChild(f); // add to folder view
    } else {
        mFolderModel->getRoot()->addChild(f);
    }

    // update the folder view
    mFolderModel->layoutChanged();
}

void TemplateViewWidget::handleRemoveFolder()
{
    qDebug() << "folder should be removed!";
}

void TemplateViewWidget::handleCopyFolder()
{
    if(currentFolderIndex.isValid()) {
        // get the parent folder
        currentFolder =
                static_cast<FolderItem*>(mFolderModel->itemFromIndex(currentFolderIndex));
    } else {
        // reset the copy selection
        currentFolder = NULL;
    }
}

void TemplateViewWidget::handlePasteFolder()
{
    // do we have something copied?
    if(currentFolder) {

        // find out where to paste the folder
        if(currentFolderIndex.isValid()) {
            // first get the new folder parent
            FolderItem *parent =
                    static_cast<FolderItem*>(mFolderModel->itemFromIndex(currentFolderIndex));

            // then check if our folder currently has one
            if(!currentFolder->getFolderID().isEmpty()) {
                FolderItem *currParent = globs::folderHash.value(currentFolder->getFolderID());
                currParent->getFolderRoot()->removeChild(currentFolder);
                currParent->removeChild(currentFolder);
            }

            // add a new sub folder
            currentFolder->setFolderID(parent->getID()); // set its parent folder id
            parent->getFolderRoot()->addChild(currentFolder); // add to thumb view
            parent->addChild(currentFolder); // add to folder view
        }

    }

    // update the folder view
    mFolderModel->layoutChanged();
}

