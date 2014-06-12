#include "elementlistwidget.h"
#include "ui_elementlistwidget.h"

ElementListWidget::ElementListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ElementListWidget)
{
    ui->setupUi(this);
}

ElementListWidget::~ElementListWidget()
{
    delete ui;
}
