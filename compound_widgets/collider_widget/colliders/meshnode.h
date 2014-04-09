#ifndef MESHNODE_H
#define MESHNODE_H

#include <QGraphicsItem>
#include <QColor>
#include <QRectF>
#include <QSize>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>

class MeshNode : public QGraphicsItem
{

public:
    MeshNode(QGraphicsItem *parent);

    // QGraphicsItem interface
    QRectF boundingRect() const;
    QPainterPath shape() const;
    int type() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QColor outlineColor() const;
    void setOutlineColor(const QColor &color);
    QColor fillColor() const;
    void setFillColor(const QColor &color);
    QVariant itemChange (GraphicsItemChange change, const QVariant &value);


private:
    QColor mOutlineColor, mFillColor, mSelectedColor;
    QRectF *mRect;


    // QGraphicsItem interface

};

#endif // MESHNODE_H
