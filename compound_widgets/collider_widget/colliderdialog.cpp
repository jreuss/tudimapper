#include "colliderdialog.h"
#include "ui_colliderdialog.h"

ColliderDialog::ColliderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColliderDialog)
{
    ui->setupUi(this);
}

ColliderDialog::ColliderDialog(ItemTemplate* item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColliderDialog)
{
    ui->setupUi(this);
    ui->widget->import = false;
    ui->widget->setupConnections();
    ui->widget->onLoadSelectedItem(item);
}

ColliderDialog::~ColliderDialog()
{
    delete ui;
}
void ColliderDialog::reject(){

    ui->widget->closeWithOutFinishMesh();
    QDialog::reject();
}

