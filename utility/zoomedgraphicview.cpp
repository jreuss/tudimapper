#include "zoomedgraphicview.h"

ZoomedGraphicView::ZoomedGraphicView(QWidget *parent) :
    QGraphicsView(parent)
{
    setDragMode(ScrollHandDrag);

    stepX = 50;
    stepY = 50;
    pX=0;
    pY=0;
    showGrid = false;
    snapToGrid = false;
}

void ZoomedGraphicView::wheelEvent(QWheelEvent *event)
{
    double degrees = -event->delta() / 8;
    double steps = degrees / 15;
    zoomFactor = std::pow(1.125, steps);

    scale(zoomFactor, zoomFactor);

    emit(zoomChanged(this->transform().m11()));
    viewport()->update();
}

double ZoomedGraphicView::getZoomFactor() const
{
    return zoomFactor;
}

void ZoomedGraphicView::setZoomFactor(double value)
{
    zoomFactor = value;
}

void ZoomedGraphicView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    viewport()->update();
}

void ZoomedGraphicView::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawForeground(painter, rect);

    if(!showGrid){
        return;
    }

    QPen pen(QColor(Qt::gray));
    pen.setWidth(0);

    painter->setPen(pen);
    painter->setOpacity(0.75);
    // draw horizontal grid
    int y = (rect.top()+pY)/stepY;
    qreal start = y*stepY;

    if (start > rect.top()) {
        start -= stepY;
    }

    for (qreal y = start - stepY; y < rect.bottom(); ) {
        y += stepY;
        painter->drawLine(rect.left(), y, rect.right(), y);
    }

    // now draw vertical grid
    int x = (rect.left()+pX)/stepX;
    start = x*stepX;

    if (start > rect.left()) {
        start -= stepX;
    }

    for (qreal x = start - stepX; x < rect.right(); ) {
        x += stepX;
        painter->drawLine(x, rect.top(), x, rect.bottom());
    }
}

void ZoomedGraphicView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx,dy);
    pX += dx;
    pY += dy;
    viewport()->update();
}

void ZoomedGraphicView::handleShowGrid(bool value)
{
    showGrid = value;
    if(!value) {
        snapToGrid = false;
    }

    viewport()->update();
}

void ZoomedGraphicView::handleSetGridOffsetX(int value)
{
    stepX = value;
    viewport()->update();
}

void ZoomedGraphicView::handleSetGridOffsetY(int value)
{
    stepY = value;
    viewport()->update();
}

void ZoomedGraphicView::handleSnapToGrid(bool value)
{
    if(showGrid) {
        snapToGrid = value;
    } else {
        snapToGrid = false;
    }
}
