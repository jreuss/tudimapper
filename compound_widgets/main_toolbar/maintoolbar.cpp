#include "maintoolbar.h"
#include "ui_maintoolbar.h"

MainToolBar::MainToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainToolBar)
{
    ui->setupUi(this);

    ui->spinBox_x_grid->setEnabled(false);
    ui->spinBox_y_grid->setEnabled(false);
    ui->btn_snap_grid->setEnabled(false);

    alignX = ui->alignX;
    alignY = ui->alignY;
    showGrid = ui->btn_show_grid;
    snapToGrid = ui->btn_snap_grid;
    stepX = ui->spinBox_x_grid;
    stepY = ui->spinBox_y_grid;
    connect(ui->btn_collider,SIGNAL(toggled(bool)),
            SLOT(handleColToggled(bool)));

    connect(ui->btn_translate,SIGNAL(toggled(bool)),
            SLOT(handleTransToggled(bool)));

    connect(ui->btn_scale,SIGNAL(toggled(bool)),
            SLOT(handleScaleToggled(bool)));

    connect(ui->btn_rotate,SIGNAL(toggled(bool)),
            SLOT(handleRotateToggled(bool)));
    connect(ui->btn_show_grid,SIGNAL(toggled(bool)),
            SLOT(handleGridToggled(bool)));
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
    ui->doubleSpinBox->setValue(factor * 100);
}

void MainToolBar::handleGridToggled(bool value)
{
    ui->spinBox_x_grid->setEnabled(value);
    ui->spinBox_y_grid->setEnabled(value);
    ui->btn_snap_grid->setEnabled(value);
    if(value == false){
       ui->btn_snap_grid->setChecked(false);
    }
}


