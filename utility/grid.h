#ifndef GRID_H
#define GRID_H

#include <QGraphicsItem>
#include <QDebug>
#include "stable_pch.h"

class Grid : public QGraphicsItem
{
public:
    explicit Grid();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setDimensions(QRect);
    void setPosition(int, int);
private:
    mutable QRect rect;
    int viewWidth, viewHeight;
    float scaleFactor;

signals:

public slots:

};

#endif // GRID_H
