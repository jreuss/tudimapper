#ifndef ZOOMEDGRAPHICVIEW_H
#define ZOOMEDGRAPHICVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class ZoomedGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ZoomedGraphicView(QWidget *parent = 0);

    double getZoomFactor() const;
    void setZoomFactor(double value);

signals:
    void zoomChanged(double);

public slots:

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *);
    double zoomFactor;
};

#endif // ZOOMEDGRAPHICVIEW_H
