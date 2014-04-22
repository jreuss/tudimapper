#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include "stable_pch.h"
#include "utility/imgproc.h"
#include "models/importtreemodel.h"
#include "items/itemtemplate.h"

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = 0);
    ImportDialog(QList<QUrl> imgFiles, QWidget *parent = 0);
    ~ImportDialog();

public slots:
   void handleImportItemSelectionChanged(QModelIndex);
   void handleApplySplitOption();
   void handleImportItemNameChanged(QString);
   void handleToggleTreelistEnabled();
   void handleToggleColliderButton(bool);
   void handleToggleAnimationButton(bool);
   void handleToggleSplitButton(bool);

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
