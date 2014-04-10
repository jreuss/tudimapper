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

    FolderItem *root = new FolderItem();

    for(int i=0; i<10; i++) {
        FolderItem *itm = new FolderItem(QString("Folder %1").arg(QString::number(i)));
        for(int j=0; j<100; j++) {
            ItemTemplate *tmp = new ItemTemplate(QString("Skede %1").arg(QString::number(qrand())));
            tmp->setPath(":/images/actionscale");
            itm->getFolderRoot()->addChild(tmp);
        }

        root->addChild(itm);
    }

    mFolderModel->setRoot(root);
    mTreeView->setModel(mFolderModel);

    mTemplateModel = new TemplateThumbModel(1);

    mTiledListView = new ThumbnailGridView();
    mTiledListView->setItemDelegate(new TemplateDelegate());
    mTiledListView->setModel(mTemplateModel);

    mLayout = new QHBoxLayout();
    mLayout->addWidget(mTreeView);
    mLayout->addWidget(mTiledListView);

    setLayout(mLayout);

    setupConnections();
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

void TemplateViewWidget::handleFolderSelectionChanged(QModelIndex idx)
{
    FolderItem *tmp = static_cast<FolderItem*>(mFolderModel->itemFromIndex(idx));


    mTiledListView->updateGeometry();

    mTemplateModel->setRoot(tmp->getFolderRoot());

mTiledListView->updateSomething();


}
