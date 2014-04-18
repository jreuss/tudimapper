#ifndef GRAPHICSVIEWRULERWIDGET_H
#define GRAPHICSVIEWRULERWIDGET_H

#include "stable_pch.h"

class GraphicsViewRulerWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS(RulerType)
    Q_PROPERTY(qreal origin READ origin WRITE setOrigin)
    Q_PROPERTY(qreal rulerUnit READ rulerUnit WRITE setRulerUnit)
    Q_PROPERTY(qreal rulerZoom READ rulerZoom WRITE setRulerZoom)

public:
    enum RulerType { Horizontal, Vertical };

    explicit GraphicsViewRulerWidget(GraphicsViewRulerWidget::RulerType rulerType,
                                     QWidget *parent = 0);

    QSize minimumSizeHint() const;
    GraphicsViewRulerWidget::RulerType rulerType() const;
    qreal origin() const;
    qreal rulerUnit() const;
    qreal rulerZoom() const;

    const int RULER_BREADTH;
    void setCursorPos(const QPoint cursorPos);
signals:

public slots:
    void setOrigin(const qreal origin);
    void setRulerUnit(const qreal rulerUnit);
    void setRulerZoom(const qreal rulerZoom);
    void setMouseTrack(const bool track);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    RulerType mRulerType;
    qreal mOrigin;
    qreal mRulerUnit;
    qreal mRulerZoom;
    QPoint mCursorPos;
    bool mMouseTracking;
    bool mDrawText;
    void drawAScaleMeter(QPainter *painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin);
    void drawFromOriginTo(QPainter *painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition);
    void drawMousePosTick(QPainter *painter);
};

#endif // GRAPHICSVIEWRULERWIDGET_H
