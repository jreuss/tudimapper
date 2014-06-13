#ifndef ITEMELEMENT_H
#define ITEMELEMENT_H
#include "items/abstracttreepixmapitem.h"
#include "items/itemtemplate.h"
#include "items/abstracttreegraphicsitem.h"
#include "compound_widgets/collider_widget/colliders/meshcollider.h"
#include "compound_widgets/collider_widget/colliders/circlecollider.h"
#include <mainscene.h>
#include <QDebug>


class ItemElement : public AbstractTreePixmapItem
{

public:
    enum ElementType { NORMAL, LAYER};
    ItemElement();
    ItemElement(ItemTemplate* tmp);
    ItemElement(ItemElement* element);


    QString getName() const;
    void setName(const QString &value);

    ItemTemplate *getTemplate() const;
    void setTemplate(ItemTemplate *value);

    QRectF getRect();

private:
    ElementType mType;
    QString mName;
    ItemTemplate* mTemplate;
    QRectF mColliderRect;

    //these variables are used to draw overlays for transformations and colliders
    //and are also used to make the transformations work
    bool showColliders;
    bool mRotateEnabled;

    //used to draw rotation overlay
    float mRotationSpanAngle;
    float mRotationStartAngle;
    QPointF mRotationCenter;
    bool mItemDragged;

    //used for scale so you can flip the scale
    bool mScaleEnabled;
    enum ScaleXDirection { PosXAxis, NegXAxis};
    enum ScaleYDirection { PosYAxis, NegYAxis};
    enum ScaleType { x, y};
    ScaleXDirection mScaleXDirection;
    ScaleYDirection mScaleYDirection;
    float mScaleFeedbackRectsSize;
    bool mIsValidScaleOriginPoint;
    ScaleType mScaleType;
    qreal lastDwidth;
    qreal lastDheight;
    QPointF mScaleOrigin;



signals:

    // QGraphicsItem interface
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    // QGraphicsItem interface
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public:
    QRectF boundingRect() const;

    ElementType getType() const;
    void setType(const ElementType &value);
    QIcon icon;
private:

    float viewScale;
    void updateColliderRect(QRectF tmp);
    void setDraggedRotation(QPointF pos, QPointF lastPos);
    float angleBetweenVectors(QVector2D v1, QVector2D v2);
    void drawScaleOverlay(QPainter *painter, QPen pen, QRectF outlineRect);

    QPointF getScaleOrigin(QVector2D vec, QRectF rect);
    void setNonUniformScale(QPointF pos, QPointF lastPos);
};


#endif // ITEMELEMENT_H
