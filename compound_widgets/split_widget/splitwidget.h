#ifndef SPLITWIDGET_H
#define SPLITWIDGET_H

#include "stable_pch.h"
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

    void toogleUI(bool enable);
    void setupConnections();
    void onLoadSelectedItem(ItemTemplate *item);

signals:
    void onSplitAplied(ItemTemplate*,bool,float);
    void onSplitAndAddToSceneAplied(ItemTemplate*,bool,float);
    void onToggleMainUIEnabled();
    void onDoubleClick(ItemTemplate*);

    private slots:
     void handleMatchTressholdChanged(int);
     void handleSplitAplied();
     void handleSplitAndAddToSceneAplied();
     void handleDoubleClick(ItemTemplate*);
private:
    Ui::SplitWidget *ui;
    ItemTemplate *mCurrentItem;
    ImgProc mImproc;
    QFont numFont;
    QFont numFontOutline;
    QList<QGraphicsTextItem*> numberList;
    float mShapeTreshVal;
    bool mRemoveDuplicates;
    QList<QList<unsigned> > mColorMatches;
};

#endif // SPLITWIDGET_H
