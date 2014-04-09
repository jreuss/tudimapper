#include "importdialog.h"
#include "ui_importdialog.h"


ImportDialog::ImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    mModel = new ImportTreeModel();
    ui->treeView->setModel (mModel);
    setupConnections();
}

ImportDialog::ImportDialog(QList<QUrl> imgFiles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);

    ui->spritewidget->setupConnections ();
    ui->colliderwidget->setupConnections();
    mModel = new ImportTreeModel();
    ui->treeView->setModel (mModel);

    setupConnections();
    mModel->addItemsFromUrls (imgFiles);
}

ImportDialog::~ImportDialog()
{
    delete mModel;
    delete ui;
}

void ImportDialog::handleImportItemSelectionChanged(QModelIndex index)
{
    if(index.row() != mCurrentIndex.row())
    {
        mCurrentIndex = index;

        mCurrentItm = mModel->itemFromIndex (index);

        int type = mCurrentItm->importType ();
        ui->stack_pages->setCurrentIndex(type);

        if(type == ItemTemplate::Single){
            ui->colliderwidget->onLoadSelectedItem (mCurrentItm);
        } else if(type == ItemTemplate::SpriteSheet) {
            ui->spritewidget->onLoadSelectedItem(mCurrentItm);
        }
    }

    // animate the pages when switching
    animatePage ();
}

void ImportDialog::handleApplySplitOption()
{
    if(ui->radio_split->isChecked())
    {

    }

}

void ImportDialog::handleImportItemNameChanged(QString text)
{
    QModelIndex index = ui->treeView->currentIndex ();
    if(!text.isEmpty ()) {
        mModel->setData (index,text,Qt::EditRole);
    } else {
        mModel->setData (index,QString("Item"),Qt::EditRole);
    }
}

void ImportDialog::handleToggleTreelistEnabled()
{
    ui->treeView->setEnabled (!ui->treeView->isEnabled ());
}


void ImportDialog::setupConnections()
{
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(handleImportItemSelectionChanged(QModelIndex)));

    connect(ui->btn_apply,SIGNAL(clicked()),
            this,SLOT(handleApplySplitOption()));
}

void ImportDialog::animatePage()
{
    QWidget *page = ui->stack_pages->currentWidget();
    QRect origin = page->rect();
    QRect offset = QRect(page->pos().x() - page->width(), page->pos().y(),
                         page->width(), page->height());

    QPropertyAnimation *anim = new QPropertyAnimation(page, "geometry");
    anim->setDuration(200);
    anim->setStartValue(offset);
    anim->setEndValue(origin);
    anim->setEasingCurve(QEasingCurve::OutQuad);

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

