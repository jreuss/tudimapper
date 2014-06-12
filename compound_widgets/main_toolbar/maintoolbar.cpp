#include "maintoolbar.h"
#include "ui_maintoolbar.h"

MainToolBar::MainToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainToolBar)
{
    ui->setupUi(this);

    alignX = ui->alignX;
    alignY = ui->alignY;

    connect(ui->btn_collider,SIGNAL(toggled(bool)),
            SLOT(handleColToggled(bool)));

    connect(ui->btn_translate,SIGNAL(toggled(bool)),
            SLOT(handleTransToggled(bool)));

    connect(ui->btn_scale,SIGNAL(toggled(bool)),
            SLOT(handleScaleToggled(bool)));

    connect(ui->btn_rotate,SIGNAL(toggled(bool)),
            SLOT(handleRotateToggled(bool)));
}

MainToolBar::~MainToolBar()
{
    delete ui;
}

void MainToolBar::handleColToggled(bool active)
{
    emit onColToggled(active);
}

void MainToolBar::handleTransToggled(bool active)
{
    emit onTransToggled(active);
}

void MainToolBar::handleScaleToggled(bool active)
{
    emit onScaleToggled(active);
}

void MainToolBar::handleRotateToggled(bool active)
{
    emit onRotateToggled(active);
}

void MainToolBar::handleZoomChanged(double factor)
{
    qDebug() << "factor: " << factor;
    ui->doubleSpinBox->setValue(factor * 100);
}


