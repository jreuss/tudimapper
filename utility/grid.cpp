#include "grid.h"

Grid::Grid()
{
    setZValue(3000);
}

QRectF Grid::boundingRect() const
{
    return rect;
}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int x = rect.x();
    int y = rect.y();

    scaleFactor = this->scene()->views().front()->transform().m11();
    qDebug() << scaleFactor;
    QPen pen(QColor(Qt::gray));
    pen.setWidth(0);
    painter->setPen(pen);

    // horizontal lines
    for(int i = rect.y(); i < rect.y()+viewHeight/scaleFactor+50; i += 50) {
        painter->drawLine(QPointF(x, y), QPointF(x+viewWidth/scaleFactor, y));
        y = i;
    }

    x = rect.x();
    y = rect.y();

    // vertical lines
    for(int i = rect.x(); i < rect.x()+viewWidth/scaleFactor+50; i += 50) {
        painter->drawLine(QPointF(x, y), QPointF(x, y+viewHeight/scaleFactor ));
        x = i;
    }
}

void Grid::setDimensions(QRect dim)
{
    viewWidth = dim.width();
    viewHeight = dim.height();
    rect = QRect(this->scenePos().x(),this->scenePos().y(),viewWidth,viewHeight);
}

void Grid::setPosition(int x, int y)
{
    prepareGeometryChange();
    rect.setX(rect.x()+x); rect.setY(rect.y()+y);

    qDebug() << rect;
}
