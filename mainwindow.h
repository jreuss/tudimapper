#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "stable_pch.h"
#include "mainscene.h"

#include "utility/filemanager.h"
#include "compound_widgets/import_widget/importdialog.h"
#include "elementtreemodel.h"
#include "compound_widgets/collider_widget/colliderdialog.h"
#include "leveltreemodel.h"



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
    void handleTemplatesRecieved(QPointF, QList<ItemTemplate*>);


protected slots:
    void handleSceneSelectionChanged();
    void handleTreeviewSelectionChanged(QModelIndex pressedOn);
    void handleOnItemsDeleted(QList<QGraphicsItem *>);
    void handleUpdatePropeties(QItemSelection,QItemSelection);
    void handleUpdatePos();
    void handleChangeXPos(int);
    void handleChangeYPos(int);
    void handleChangeScale(double);
    void handleChangeRotation(double);
    void handleUpdateImportOptions();
    void handleLevelChange(QItemSelection,QItemSelection);
    void handleAddLevel();
    void handleRemoveLevel();
    void handleShowCollider(bool);


private:
    FileManager mFileManager;
    ImportDialog *mImportDialog;
    //MainScene *mainScene;
    Ui::MainWindow *ui;
    ItemTemplate* tmp;
    ElementTreeModel *elementModel;
    LevelTreeModel *levelModel;

    MainScene *selectedLevel;
    MainScene *previousSelectedLevel;

    QList<ItemElement*> selectedItems;

};

#endif // MAINWINDOW_H
