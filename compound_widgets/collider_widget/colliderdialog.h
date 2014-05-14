#ifndef COLLIDERDIALOG_H
#define COLLIDERDIALOG_H

#include <QDialog>
#include <QDebug>
#include "items/itemtemplate.h"

namespace Ui {
class ColliderDialog;
}

class ColliderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColliderDialog(QWidget *parent = 0);
    ColliderDialog(ItemTemplate* item, QWidget *parent = 0);
    ~ColliderDialog();


private:
    Ui::ColliderDialog *ui;
    void reject();
};

#endif // COLLIDERDIALOG_H
