#ifndef GRAPHICSGRIDITEM_H
#define GRAPHICSGRIDITEM_H

#include "stable_pch.h"

class GraphicsGridItem : public QGraphicsItem
{
public:
    explicit GraphicsGridItem(QRectF rect=QRectF(), QGraphicsItem *parent = 0);
    ~GraphicsGridItem();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF getGridRect() const;
    void setGridRect(const QRectF &value);

    void setGrid(int mRows, int mColumns, int verticalspacing, int horizontalspacing);

    int getColumns() const;
    void setColumns(int value);

    int getRows() const;
    void setRows(int value);

    int getVSpacing() const;
    void setVSpacing(int value);

    int getHSpacing() const;
    void setHSpacing(int value);

    void updateGrid();

private:
    QRectF mRect;
    QVector<QLine> mVLines, mHLines;
    int mColumns, mRows, mVSpacing, mHSpacing;

};

#endif // GRAPHICSGRIDITEM_H
