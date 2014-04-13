#include "splitwidget.h"
#include "ui_splitwidget.h"

SplitWidget::SplitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplitWidget)
{
    ui->setupUi(this);
}

SplitWidget::~SplitWidget()
{
    delete ui;
}
