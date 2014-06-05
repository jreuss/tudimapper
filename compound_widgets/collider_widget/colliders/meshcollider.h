#ifndef MESHCOLLIDER_H
#define MESHCOLLIDER_H


#include "stable_pch.h"
#include "meshnode.h"
#include "collider.h"

class MeshCollider : public Collider
{

public:
    explicit MeshCollider(const QString &name, bool consolidated = true);

    ~MeshCollider();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape () const;

    void updatePolygon();

    int type() const;

    void setSize(const QSizeF &size);

    void addChildNode(MeshNode *child);
    void addChildrenNodes(const QList<MeshNode *> & children);
    void addChildNodeAt(int index, MeshNode *child);

    void removeChild(MeshNode *child);
    void clearChildren();

    int indexOfChild(MeshNode *child);

    int childCount() const;

    void childCalledConsolidate();

    void setConsolidated() { mConsolidated = true; }

    void setNonUniformScale(QPointF pos, QPointF lastPos);
    void setColliderRotation (QPointF pos,  QPointF rotationPoint);
    void setCanBeDeleted(bool can);
    bool canBeDeleted();
    QPolygonF getPolyToDraw();

      QList<MeshNode *> mChildNodes;

protected:


    void drawRotateOverlay(QPainter *painter, QPen pen);
    void drawPoly(QPainter *painter,const QStyleOptionGraphicsItem *option, QPen pen);
    void drawNonConsolidatedMesh(QPainter *painter, QPen pen);
    void setScaleEnabled(bool enable);

    QPointF mScaleOrigin;
    QPointF mRotationOriginPoint;
    QVector2D mRotationOrigin;
    QPointF mRotationCenter;

    bool mConsolidated;
    bool mCanBeDeleted;



    QRectF mRect;

    QPolygonF mPoly;

    QVariant itemChange (GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MESHCOLLIDER_H
