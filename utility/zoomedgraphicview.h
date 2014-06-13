#ifndef ZOOMEDGRAPHICVIEW_H
#define ZOOMEDGRAPHICVIEW_H

#include <QGraphicsView>
#include "stable_pch.h"
#include "utility/grid.h"
#include <QWheelEvent>

class ZoomedGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ZoomedGraphicView(QWidget *parent = 0);

    double getZoomFactor() const;
    void setZoomFactor(double value);
    void initGrid();
    int stepX,stepY;
    bool showGrid;

signals:
    void zoomChanged(double);

public slots:
    void handleShowGrid(bool);
    void handleSetGridOffsetX(int);
    void handleSetGridOffsetY(int);
    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *);
    double zoomFactor;
    Grid *grid;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *);
    void drawForeground(QPainter *painter, const QRectF &rect);
    int pX, pY;
    // QAbstractScrollArea interface
protected:
    void scrollContentsBy(int dx, int dy);
};

#endif // ZOOMEDGRAPHICVIEW_H
