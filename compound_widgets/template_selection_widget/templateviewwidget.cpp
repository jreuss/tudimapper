#include "templateviewwidget.h"
#include "ui_templateviewwidget.h"

#include <QStandardItemModel>

TemplateViewWidget::TemplateViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemplateViewWidget)
{
    ui->setupUi(this);

    // create folder model
    mFolderModel = new TemplateFolderViewModel(1);
    mTreeView = new QTreeView();

    mTreeView->setModel(mFolderModel);

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

    setupConnections();

//    for(int i=0; i< 10; i++){
//        int j = qrand() % 20 + 1;
//        FolderItem* itm = new FolderItem(QString("Folder %1").arg(j));
//        for(int k = 0; k<j; k++)
//        {
//           FolderItem* itm2 = new FolderItem(QString("Folder %1").arg(1+qrand()));
//           itm->addChild(itm2);
//        }
//        mFolderModel->getRoot()->addChild(itm);
//    }
}

void TemplateViewWidget::setupConnections()
{
    connect(mTreeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(handleFolderSelectionChanged(QModelIndex)));
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
    FolderItem *itm = new FolderItem("Imported");
    itm->getFolderRoot()->addChildren(templateRoot->getChildren());

    mFolderModel->getRoot()->addChild(itm);
    FolderItem *child = new FolderItem("Skede");
    itm->addChild(child);
   itm->getFolderRoot()->addChild(child);
   child->setParent(itm);
    mFolderModel->layoutChanged();

}

void TemplateViewWidget::handleFolderSelectionChanged(QModelIndex idx)
{
    FolderItem *tmp = static_cast<FolderItem*>(mFolderModel->itemFromIndex(idx));
    mTemplateModel->setRoot(tmp->getFolderRoot());
    mTiledListView->updateSomething();
}

