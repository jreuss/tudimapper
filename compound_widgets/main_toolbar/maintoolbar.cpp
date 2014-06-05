#include "maintoolbar.h"
#include "ui_maintoolbar.h"

MainToolBar::MainToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainToolBar)
{
    ui->setupUi(this);
    connect(ui->btn_collider,SIGNAL(toggled(bool)),
            SLOT(handleColToggled(bool)));
}

MainToolBar::~MainToolBar()
{
    delete ui;
}

void MainToolBar::handleColToggled(bool active)
{
    emit onColToggled(active);
}
