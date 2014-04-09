#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createConnections()
{
    connect (ui->actionQuit, SIGNAL (triggered()),
             qApp, SLOT (quit()));
    connect (ui->actionImportSpecial, SIGNAL(triggered()),
             this, SLOT (handleImportSpecial()));
}

void MainWindow::handleImportSpecial()
{
    QList<QUrl> urls = mFileManager.FileDialog();

    if (urls.count() != 0)
    {
        mImportDialog = new ImportDialog(urls, this);
        mImportDialog->setModal(true);
        mImportDialog->exec();
    }
}
