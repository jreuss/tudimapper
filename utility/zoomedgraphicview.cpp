#include "zoomedgraphicview.h"

ZoomedGraphicView::ZoomedGraphicView(QWidget *parent) :
    QGraphicsView(parent)
{
    setDragMode(ScrollHandDrag);
}

void ZoomedGraphicView::wheelEvent(QWheelEvent *event)
{

    double degrees = -event->delta() / 8;
    double steps = degrees / 15;
    zoomFactor = std::pow(1.125, steps);

    scale(zoomFactor, zoomFactor);

    emit(zoomChanged(this->transform().m11()));

}
double ZoomedGraphicView::getZoomFactor() const
{
    return zoomFactor;
}

void ZoomedGraphicView::setZoomFactor(double value)
{
    zoomFactor = value;
}


