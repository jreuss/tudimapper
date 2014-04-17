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
    folderView->hideColumn(1);

    thumbView->setModel(model);
    folderView->setModel(folderProxy);

    hLayout->addWidget(folderView);
    hLayout->addWidget(thumbView);

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
    ItemTemplate *item = new ItemTemplate("Name", ItemTemplate::Folder, model->getRoot());

    if(currentModelIndex.isValid()) {
        model->insertItem(currentModelIndex, item);
    } else {
        model->insertItem(model->getRoot()->getChildren().count(), model->getRoot(), item);
    }

    // bad logic
    thumbView->forceUpdate();
}

void TemplateFolderWidget::handleAddTemplateAction()
{
    if(currentModelIndex.isValid()) {
        ItemTemplate *item = new ItemTemplate("Name", ItemTemplate::Folder, model->getRoot());
        model->insertItem(currentModelIndex, item);
    }
    thumbView->forceUpdate();
}

void TemplateFolderWidget::handleRemoveAction()
{
    if(currentModelIndex.isValid()) {
        model->removeItem(currentModelIndex);
    }
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
