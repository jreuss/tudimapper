#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include "stable_pch.h"
#include "utility/imgproc.h"
#include "models/importtreemodel.h"
#include "items/itemtemplate.h"

#include <QDebug>
#include <QMessageBox>

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = 0);
    ImportDialog(QList<QUrl> imgFiles, QWidget *parent = 0);
    ImportDialog(QList<ItemTemplate*> tempsToChange, QWidget *parent =0);
    ~ImportDialog();

public slots:
   void handleImportItemSelectionChanged(QModelIndex);
   void handleApplyGroupOption();
   void handleImportItemNameChanged(QString);
   void handleToggleMainUIEnabled();
   void handleToggleColliderButton(bool);
   void handleToggleAnimationButton(bool);
   void handleToggleSplitButton(bool);
   void handleRemoveSplitParent();
   void handleExpandSplit(ItemTemplate*);
   void handleSelectFirstUrl(ItemTemplate *);
   void handleSplitSceneDoubleClick(ItemTemplate *);

   void accept();

signals:
    ItemTemplate *onImportAccept(ItemTemplate *);

private:
    void parseUrls(QList<QUrl> urls);
    void setupConnections();
    void animatePage();
    void toogleToolButtons();

    QGraphicsScene *mScene;
    Ui::ImportDialog *ui;
    ImgProc mImgProc;
    QBrush mCheckerbrush;
    QModelIndex mCurrentIndex;
    ImportTreeModel *mModel;
    ItemTemplate *mCurrentItm;

};

#endif // IMPORTDIALOG_H
