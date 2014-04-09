#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QUrl>
#include <QList>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QString>
#include <QUuid>
#include <QHash>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <opencv2/core/core.hpp>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include <QDir>
#include <QPropertyAnimation>
#include <QRegExp>
#include <QGraphicsItemGroup>
#include <QGraphicsProxyWidget>
#include <QGridLayout>
#include <QLayout>

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

private:
    void parseUrls(QList<QUrl> urls);
    void setupConnections();
    void animatePage();

    QGraphicsScene mScene;
    Ui::ImportDialog *ui;
    ImgProc mImgProc;
    QBrush mCheckerbrush;
    QModelIndex mCurrentIndex;
    ImportTreeModel *mModel;
    ItemTemplate *mCurrentItm;
};

#endif // IMPORTDIALOG_H
