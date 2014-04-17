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
    mTreeView->setAcceptDrops(true);
    mTreeView->setDragEnabled(true);

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
            this, SLOT(handleOnTemplateDropped(QStringList, AbstractTreeItem*)));

    connect(mFolderModel, SIGNAL(onItemDropped(QStringList, AbstractTreeItem*)),
            this, SLOT(handleOnFolderDropped(QStringList, AbstractTreeItem*)));

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

    folderContextMenu.addSeparator();

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
    // update model
    mFolderModel->layoutChanged();
}

void TemplateViewWidget::moveFolder(FolderItem *src, FolderItem *dst)
{
    // check if our folder currently has parent folder
    if(!src->getFolderID().isEmpty()) {
        FolderItem *currParent = globs::folderHash.value(src->getFolderID());
        currParent->getFolderRoot()->removeChild(src);
        currParent->removeChild(src);
    } else {
        mFolderModel->getRoot()->removeChild(src);
    }

    // add a new sub folder
    src->setFolderID(dst->getID()); // set its parent folder id
    dst->getFolderRoot()->addChild(src); // add to thumb view
    dst->addChild(src); // add to folder view
}

void TemplateViewWidget::handleFolderSelectionChanged(QModelIndex idx)
{
    FolderItem *tmp = static_cast<FolderItem*>(mFolderModel->itemFromIndex(idx));
    mTemplateModel->setRoot(tmp->getFolderRoot());
    mTiledListView->forceUpdate();
}

void TemplateViewWidget::handleOnTemplateDropped(QStringList itemIDs, AbstractTreeItem* parent)
{
    FolderItem *targetFolder = static_cast<FolderItem*>(parent);

    foreach(QString s, itemIDs) {
        AbstractTreeItem *itm = globs::templateHash.value(s);
        if(itm->getItemType() == AbstractTreeItem::TemplateType) {
            targetFolder->switchItemToThisFolder(itm);
        }
    }
}

void TemplateViewWidget::handleOnFolderDropped(QStringList folderIDs, AbstractTreeItem *parent)
{
    FolderItem *targetFolder = static_cast<FolderItem*>(parent);

    foreach(QString s, folderIDs) {
        AbstractTreeItem *itm = globs::folderHash.value(s);

        // first off, find out whether we have a folder item dropped
        if(itm->getItemType() == AbstractTreeItem::FolderType) {
            moveFolder(static_cast<FolderItem*>(itm), targetFolder);
        }
    }

    mFolderModel->layoutChanged();
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

    // insert the folder into the folder hash
    globs::folderHash.insert(f->getID(), f);

    // update the folder view
    mFolderModel->layoutChanged();
}

void TemplateViewWidget::handleRemoveFolder()
{
    // reset the template view, so that we dont get NULL reference when the childs
    // are removed
    if(currentFolderIndex.isValid()) {
        mTemplateModel->reset();

        // remove the desired row
        AbstractTreeItem *child = mFolderModel->itemFromIndex(currentFolderIndex);
        QModelIndex parent = mFolderModel->indexFromItem(child->getParent());
        mFolderModel->removeRow(currentFolderIndex.row(), parent);
        // update views
        mTemplateModel->layoutChanged();
    }
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

        // check if our folder currently has parent folder
        if(!currentFolder->getFolderID().isEmpty()) {
            FolderItem *currParent = globs::folderHash.value(currentFolder->getFolderID());
            currParent->getFolderRoot()->removeChild(currentFolder);
            currParent->removeChild(currentFolder);
        } else {
            mFolderModel->getRoot()->removeChild(currentFolder);
        }

        // find out where to paste the folder
        if(currentFolderIndex.isValid()) {
            // first get the new folder parent
            FolderItem *parent =
                    static_cast<FolderItem*>(mFolderModel->itemFromIndex(currentFolderIndex));

            // add a new sub folder
            currentFolder->setFolderID(parent->getID()); // set its parent folder id
            parent->getFolderRoot()->addChild(currentFolder); // add to thumb view
            parent->addChild(currentFolder); // add to folder view
        } else {
            currentFolder->setFolderID("");
            mFolderModel->getRoot()->addChild(currentFolder);
        }
    }

    currentFolder = NULL;
    // update the folder view
    mFolderModel->layoutChanged();
}

