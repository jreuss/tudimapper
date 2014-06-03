#include "templatefolderwidget.h"
#include "ui_templatefolderwidget.h"

TemplateFolderWidget::TemplateFolderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemplateFolderWidget)
{
    ui->setupUi(this);
    hLayout = new QHBoxLayout();

    thumbView = new ThumbnailGridView();
    folderView = new QTreeView();

    model = new TemplateModel(2);

    folderProxy = new FolderProxy();
    folderProxy->setSourceModel(model);
    folderView->setContextMenuPolicy(Qt::CustomContextMenu);


    thumbView->setModel(model);
    thumbView->setItemDelegate(new TemplateDelegate());
    folderView->setModel(folderProxy);
    folderView->hideColumn(1);

    thumbView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    folderView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    folderView->setMaximumWidth(260);

    splitter = new QSplitter();

    splitter->addWidget(folderView);
    splitter->addWidget(thumbView);

    hLayout->addWidget(splitter);


    setLayout(hLayout);

    createActions();

    // setup connection for context menu
    connect(folderView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(handleContextMenuRequested(QPoint)));

    connect(folderView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(handleFolderSelectionChanged(QModelIndex)));

    connect(this, SIGNAL(mapProxySelection(QModelIndex)),
            thumbView, SLOT(folderSelectionChanged(QModelIndex)));
}

TemplateFolderWidget::~TemplateFolderWidget()
{
    delete ui;
}

void TemplateFolderWidget::createActions()
{
    QAction *addFolderAction = new QAction(tr("New folder"), this);
    connect(addFolderAction, SIGNAL(triggered()), this, SLOT(handleAddFolderAction()));
    modelContextMenu.addAction(addFolderAction);

    QAction *addTemplateAction = new QAction(tr("New template"), this);
    connect(addTemplateAction, SIGNAL(triggered()), this, SLOT(handleAddTemplateAction()));
    modelContextMenu.addAction(addTemplateAction);

    QAction *removeAction = new QAction(tr("Remove"), this);
    connect(removeAction, SIGNAL(triggered()), this, SLOT(handleRemoveAction()));
    modelContextMenu.addAction(removeAction);
}

void TemplateFolderWidget::handleAddFolderAction()
{
    ItemTemplate *item = new ItemTemplate("New Folder",ItemTemplate::Folder,model->getRoot());

    if(currentModelIndex.isValid()) {
        model->insertItem(currentModelIndex, item);
    } else {
        model->insertItem(model->getRoot()->getChildren().count(), model->getRoot(), item);
    }


    // bad logic
    // thumbView->forceUpdate();
}

void TemplateFolderWidget::handleAddTemplateAction()
{
    if(currentModelIndex.isValid()) {
        ItemTemplate *item = new ItemTemplate("New Folder",ItemTemplate::Single,model->getRoot());
        model->insertItem(currentModelIndex, item);
    }
    // thumbView->forceUpdate();
}

void TemplateFolderWidget::handleRemoveAction()
{

    if(currentModelIndex.isValid()) {
        model->removeItem(currentModelIndex);
    }
    //thumbView->forceUpdate();

}

void TemplateFolderWidget::handleContextMenuRequested(QPoint position)
{
    currentModelIndex = folderProxy->mapToSource(folderView->indexAt(position));
    emit mapProxySelection(currentModelIndex);
    modelContextMenu.popup(folderView->viewport()->mapToGlobal(position));
}


void TemplateFolderWidget::handleFolderSelectionChanged(QModelIndex index)
{
    emit mapProxySelection(folderProxy->mapToSource(index));
}

void TemplateFolderWidget::addTemplates(ItemTemplate *templateRoot)
{
    // create new top-most folder
    ItemTemplate *itm = new ItemTemplate("Imported",ItemTemplate::Folder,model->getRoot() );

    model->insertItem(model->getRoot()->getChildren().count(),
                      model->getRoot(),itm);

    for(int i = 0; i < templateRoot->mChildren.count(); i++) {
        ItemTemplate* currentItem = static_cast<ItemTemplate*>(templateRoot->mChildren.at(i));
        model->insertItem(itm->getChildren().count(),
                          itm,
                          currentItem);

        if(currentItem->importType() == ItemTemplate::Split){
            for(int j = 0; j < currentItem->mChildren.count(); j++){
                model->insertItem(itm->getChildren().count(),
                                  itm,
                                  currentItem->mChildren.at(j));
            }

        }
    }
    QModelIndex idx = model->indexFromItem(itm);
    folderView->selectionModel()->clearSelection();
    folderView->selectionModel()->setCurrentIndex(idx,QItemSelectionModel::Select);
    handleFolderSelectionChanged(folderProxy->mapFromSource(idx));
}

void TemplateFolderWidget::handleRequestedTemplates(QPointF pos)
{
    QList<ItemTemplate*> templatesToSend;

    foreach(QModelIndex index, thumbView->selectionModel()->selectedIndexes() )
    {
        if (index.isValid()) {
            ItemTemplate *item = static_cast<ItemTemplate*>(model->itemFromIndex (index));
            if(item->importType() != ItemTemplate::Folder){
                templatesToSend.append(item);
            }
        }
    }
    emit onSendRequestedTemplates(pos,templatesToSend);
}

