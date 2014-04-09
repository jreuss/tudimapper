#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <QGraphicsItem>
#include <QColor>
#include <QRectF>
#include <QSize>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QVector2D>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtMath>
#include <QTransform>

#include "collider.h"

class BoxCollider : public Collider
{
public:
    BoxCollider(const QString &name);


    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void drawRotateOverlay(QPainter *painter, QPen pen);
    void drawBox(QPainter *painter, const QStyleOptionGraphicsItem *option, QPen pen);

    int type() const;

    void setRect(const QRectF &rect);
    QRectF rect() const;

    void setNonUniformScale(QPointF pos,QPointF lastPos);
    QPointF rotatePoint(QPointF ptn, float rot);
    void setColliderRotation (QPointF pos, QPointF rotationPoint);
    void setFixedRotation(float rot);
    float getMRotation() const;
    void setMRotation(float value);


    float getWidth();
    qreal getHeight();

    void setScaleEnabled(bool enable);




    float mRotationStartAngle;
    float mRotationSpanAngle;

    QVector2D mRotationOrigin;
    QPointF mRotationCenter;




protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    //THIS SHOULD MAYBE BE DELETED IT IS ONLY USED IN CONSTRUCTOR AND SETSIZE FUNTIONS
    QRectF mRect;
    QPolygonF mRectPoly;



};

#endif // BOXCOLLIDER_H
