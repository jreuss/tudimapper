#ifndef SPLITWIDGET_H
#define SPLITWIDGET_H

#include <QWidget>

#include "items/itemtemplate.h"
#include "utility/imgproc.h"

namespace Ui {
class SplitWidget;
}

class SplitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SplitWidget(QWidget *parent = 0);
    ~SplitWidget();

    void setupConnections();
    void onLoadSelectedItem(ItemTemplate *item);

signals:
    void onSplitAplied(ItemTemplate*,bool,float);

private slots:
     void handleMatchTressholdChanged(int);
     void handleSplitAplied();
private:
    Ui::SplitWidget *ui;
    ItemTemplate *mCurrentItem;
    ImgProc mImproc;
    QFont numFont;
    QFont numFontOutline;
    QList<QGraphicsTextItem*> numberList;
    float mShapeTreshVal;
    bool mRemoveDuplicates;
};

#endif // SPLITWIDGET_H