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
    ItemElement(ItemTemplate* tmp);
    ItemElement(ItemElement* element);


    QString getName() const;
    void setName(const QString &value);

    ItemTemplate *getTemplate() const;
    void setTemplate(ItemTemplate *value);

private:
    QString mName;
    ItemTemplate* mTemplate;
    QRectF mColliderRect;

    //these variables are used to draw overlays for transformations and colliders
    //and are also used to make the transformations work
    bool showColliders;
    bool mRotateEnabled;
    bool mItemDragged;
signals:

    // QGraphicsItem interface
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    // QGraphicsItem interface
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    void updateColliderRect(QRectF tmp);
    void setDraggedRotation(QPointF pos, QPointF lastPos);
    float angleBetweenVectors(QVector2D v1, QVector2D v2);
};


#endif // ITEMELEMENT_H
