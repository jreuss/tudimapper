#ifndef COLLIDER_H
#define COLLIDER_H

#include "stable_pch.h"
#include "items/abstracttreegraphicsitem.h"

class Collider : public AbstractTreeGraphicsItem
{
public:
    Collider();

    virtual void setNonUniformScale(QPointF pos, QPointF lastPos) = 0;
    virtual void setColliderRotation(QPointF pos, QPointF rotationPoint) = 0;

    virtual void setScaleEnabled(bool enable) = 0;
    bool getScaleEnabled();
    void setRotateEnabled(bool enable);

    bool getIsTrigger() const;
    void setIsTrigger(bool value);

    QString getName() const;
    void setName(const QString &value);


    void drawScaleOverlay(QPainter *painter, QPen pen,  QRectF outlineRect);
    QPointF getScaleOrigin(QVector2D vec, QRectF rect);


    //CHECK ALL BELOW AS THEY MAY NOT BE USED
    //---------------------------------------
    //Functions for vector calculation
    QVector2D vectorProjection(QVector2D v1, QVector2D v2);
    float angleBetweenVectors(QVector2D v1, QVector2D v2);

    //Used to rotate a single a point.
    QPointF rotatePoint(QPointF ptn, float rot);
    //Used to set up the scalation GUI
    void setUpScaleGui();




protected:

    enum ScaleXDirection { PosXAxis, NegXAxis};
    enum ScaleYDirection { PosYAxis, NegYAxis};
    enum ScaleType {OnlyX, OnlyY, BothXY, Uniform};
    int mScaleXScalar;
    int mScaleYScalar;
    ScaleXDirection mScaleXDirection;
    ScaleYDirection mScaleYDirection;
    ScaleType mScaleType;
    bool mScaleEnabled;
    bool mRotateEnabled;
    bool mUniformScaleEnabled;

    bool mIsValidScaleOriginPoint;
    int mScaleFeedbackRectsSize;

    bool mItemDragged;

    //Variables used for scaling and primarily the GUI
    QPointF mScaleOrigin;
    QPointF mXScalePoint;
    QPointF mYScalePoint;
    QPointF mUniformScalePoint;
    QVector2D mScaleXAxis;
    QVector2D mScaleYAxis;
    QPolygonF mScaleXPoly;
    QPolygonF mScaleYPoly;
    QPolygonF mScaleUniformPoly;
    QVector2D mUniformXScaleVector;
    QVector2D mUniformYScaleVector;
    qreal lastDwidth;
    qreal lastDheight;
    //The Rotation of an object
    float mRotation;

    //Ceneral Collider Properies
     bool mIsTrigger;
     QString mName;

     // QGraphicsItem interface
protected:
     void keyPressEvent(QKeyEvent *event);
     void keyReleaseEvent(QKeyEvent *event);
};

#endif // COLLIDER_H
