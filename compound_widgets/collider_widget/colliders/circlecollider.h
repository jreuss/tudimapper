#ifndef CIRCLECOLLIDER_H
#define CIRCLECOLLIDER_H

#include "stable_pch.h"
#include "collider.h"
//#include "utility/imgproc.h"


class CircleCollider : public Collider
{
public:
    CircleCollider(const QString &name);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void drawCircle(QPainter *painter,const QStyleOptionGraphicsItem *option, QPen pen);

    QPainterPath shape () const;

     QRectF rect() const;
     void setNonUniformScale(QPointF pos, QPointF lastPos);
     void setColliderRotation(QPointF pos, QPointF rotationPoint) {
         Q_UNUSED(pos); Q_UNUSED(rotationPoint); };

     void setScaleEnabled(bool enable);


   int type() const;

   QPointF getCenter() const;
   void setCenter(const QPointF &value);
   float getRadius() const;
   void setRadius(float value);


   void mousePressEvent(QGraphicsSceneMouseEvent *event);
   void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
   void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:

    QPointF mCenter;

    float mRadius;


};

#endif // CIRCLECOLLIDER_H
