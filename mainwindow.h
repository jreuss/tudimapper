#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDropEvent>

#include "utility/filemanager.h"
#include "compound_widgets/import_widget/importdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void createConnections();
    ~MainWindow();

public slots:
    void handleImportSpecial();
    void handleImportAccepted(ItemTemplate *item);

private:
    FileManager mFileManager;
    ImportDialog *mImportDialog;
    QGraphicsScene *mainScene;
    Ui::MainWindow *ui;
    ItemTemplate* tmp;

};

#endif // MAINWINDOW_H
