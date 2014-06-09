#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSettings>
#include <QPushButton>

namespace Ui {
class LayoutWidget;
}

class LayoutWidget : public QDialog
{
    Q_OBJECT

    int MAX_STORED_LAYOUTS;
public:
    explicit LayoutWidget(QWidget *parent = 0);
    ~LayoutWidget();
    void updateLayouts();

    // get the current stored layouts
    QList<QPair<QString, QByteArray> > getLayouts();

public slots:
    void accept();

private slots:
    void handleValidateName(QString);

private:
    int layoutsLength;
    Ui::LayoutWidget *ui;
    QList<QPair<QString, QByteArray> > layouts;
};

#endif // DIALOG_H
