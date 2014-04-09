#include "templateviewwidget.h"
#include "ui_templateviewwidget.h"

#include <QStandardItemModel>

TemplateViewWidget::TemplateViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemplateViewWidget)
{
    ui->setupUi(this);

    // create folder model
    folderModel = new TemplateFolderViewModel(2);
    treeView = new QTreeView();

    FolderItem *root = new FolderItem();

    for(int i=0; i<10; i++) {
        FolderItem *itm = new FolderItem(QString("Folder %1").arg(QString::number(i)));
        root->addChild(itm);
    }

    folderModel->setRoot(root);
    treeView->setModel(folderModel);

    model = new QStandardItemModel;
    populateModel(model);
    tiledListView = new ThumbnailGridView();
    tiledListView->setModel(model);
    tiledListView->setItemDelegate(new TemplateDelegate());

    layout = new QHBoxLayout();
    layout->addWidget(treeView);
    layout->addWidget(tiledListView);

    setLayout(layout);
}

void TemplateViewWidget::populateModel(QStandardItemModel *model)
{

    foreach (const QString &name, QStringList()
             << "Barack Obama" << "George W. Bush" << "Bill Clinton"
             << "George H. W. Bush" << "Ronald Reagan"
             << "Jimmy Carter" << "Gerald Ford" << "Richard Nixon"
             << "Lyndon B. Johnson" << "John F. Kennedy"
             << "Dwight D. Eisenhower" << "Harry S. Truman"
             << "Franklin D. Roosevelt" << "Herbert Hoover"
             << "Calvin Coolidge" << "Warren G. Harding"
             << "Woodrow Wilson" << "William Howard Taft"
             << "Theodore Roosevelt" << "William McKinley"
             << "Grover Cleveland" << "Benjamin Harrison"
             << "Grover Cleveland" << "Chester A. Arthur"
             << "James A. Garfield" << "Rutherford B. Hayes"
             << "Ulysses S. Grant" << "Andrew Johnson"
             << "Abraham Lincoln" << "James Buchanan"
             << "Franklin Pierce" << "Millard Fillmore"
             << "Zachary Taylor" << "James K. Polk" << "John Tyler"
             << "William Henry Harrison" << "Martin Van Buren"
             << "Andrew Jackson" << "John Quincy Adams"
             << "James Monroe" << "James Madison"
             << "Thomas Jefferson" << "John Adams"
             << "George Washington" << "Barack Obama" << "George W. Bush" << "Bill Clinton"
             << "George H. W. Bush" << "Ronald Reagan"
             << "Jimmy Carter" << "Gerald Ford" << "Richard Nixon"
             << "Lyndon B. Johnson" << "John F. Kennedy"
             << "Dwight D. Eisenhower" << "Harry S. Truman"
             << "Franklin D. Roosevelt" << "Herbert Hoover"
             << "Calvin Coolidge" << "Warren G. Harding"
             << "Woodrow Wilson" << "William Howard Taft"
             << "Theodore Roosevelt" << "William McKinley"
             << "Grover Cleveland" << "Benjamin Harrison"
             << "Grover Cleveland" << "Chester A. Arthur"
             << "James A. Garfield" << "Rutherford B. Hayes"
             << "Ulysses S. Grant" << "Andrew Johnson"
             << "Abraham Lincoln" << "James Buchanan"
             << "Franklin Pierce" << "Millard Fillmore"
             << "Zachary Taylor" << "James K. Polk" << "John Tyler"
             << "William Henry Harrison" << "Martin Van Buren"
             << "Andrew Jackson" << "John Quincy Adams"
             << "James Monroe" << "James Madison"
             << "Thomas Jefferson" << "John Adams"
             << "George Washington") {
        QStandardItem *item = new QStandardItem();
        //item->setIcon(ico);
        item->setText(name);
        //item->setIcon(ico);
        item->setTextAlignment(Qt::AlignCenter);

        model->appendRow(item);
    }
}

TemplateViewWidget::~TemplateViewWidget()
{
    delete model;
    delete ui;
}
