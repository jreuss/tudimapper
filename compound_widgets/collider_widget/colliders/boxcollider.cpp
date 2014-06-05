#include "boxcollider.h"
#include "collider.h"

BoxCollider::BoxCollider(const QString &name)
{
    mRect = QRectF(0,0,10,10);


    mScaleEnabled = false;
    mRotateEnabled = false;
    setFlags (ItemIsSelectable | ItemIsMovable |ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);

    setAcceptHoverEvents (true);

    mIsTrigger = false;
    mName = name;
}

QRectF BoxCollider::boundingRect() const
{

    QRectF tmpRect = rect();
    return QRectF(QPointF(tmpRect.topLeft().x()-mScaleFeedbackRectsSize,
                          tmpRect.topLeft().y()-mScaleFeedbackRectsSize),
                  QPointF(tmpRect.bottomRight().x()+mScaleFeedbackRectsSize,
                          tmpRect.bottomRight().y()+mScaleFeedbackRectsSize));

}

QPainterPath BoxCollider::shape() const
{
    QPainterPath path;

    path.addRect (rect());

    return path;

}

void BoxCollider::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget __attribute__((unused)))
{
    QPen pen(QColor(Qt::cyan));
    pen.setWidth (1);
    painter->setPen (pen);

    drawBox(painter, option, pen);

    if(mScaleEnabled) {
        drawScaleOverlay(painter,pen,rect());
    }

    if(mRotateEnabled){
        drawRotateOverlay(painter,pen);
    }
}

void BoxCollider::drawRotateOverlay(QPainter *painter, QPen pen)
{
    Q_UNUSED(pen);

    painter->drawEllipse(mRotationCenter,50,50);
    if(mItemDragged){
        painter->setPen(QColor(Qt::darkGreen));
        painter->setBrush(QColor(Qt::darkGreen));
        painter->drawPie(QRectF(mRotationCenter-QPointF(50,-50),mRotationCenter+QPointF(50,-50)),mRotationStartAngle*16,mRotationSpanAngle*16);
    }

}

void BoxCollider::drawBox(QPainter *painter, const QStyleOptionGraphicsItem *option,QPen pen){
    pen.setColor(QColor(Qt::cyan));
    if(option->state & QStyle::State_Selected){
        pen.setColor (QColor(Qt::blue));
        pen.setWidth (2);
    }

    painter->setPen (pen);
    painter->drawPolygon(mRectPoly);

    painter->setBrush (QColor(Qt::green));
    painter->setOpacity (0.3);
    painter->drawPolygon(mRectPoly);

}

int BoxCollider::type() const
{
    return QGraphicsItem::UserType+1;
}

void BoxCollider::setNonUniformScale(QPointF pos,QPointF lastPos)
{
    Q_UNUSED(lastPos);

    qreal dx, dy;
    qreal sx, sy;

    dx = mScaleOrigin.x();
    dy = mScaleOrigin.y();
    qreal dwidth = abs(pos.x ()-dx);
    if((mScaleXDirection == PosXAxis && pos.x() > dx) || (mScaleXDirection == PosXAxis && dwidth == 0)){
        dwidth =-2;
        mScaleXDirection = NegXAxis;
    } else if((mScaleXDirection == NegXAxis && pos.x() < dx) || (mScaleXDirection == NegXAxis && dwidth == 0)) {
        dwidth = -2;
        mScaleXDirection = PosXAxis;
    }

    qreal dheight = abs(pos.y()-dy);
    if((mScaleYDirection == PosYAxis && pos.y() > dy) || (mScaleYDirection == PosYAxis && dheight == 0)){
        dheight =-2;
        mScaleYDirection = NegYAxis;
    } else if((mScaleYDirection == NegYAxis && pos.y() < dy) || (mScaleYDirection == NegYAxis && dheight == 0)) {
        dheight = -2;
        mScaleYDirection = PosYAxis;
    }
    if(mScaleType == BothXY){
        sx = dwidth/rect().width ();
        sy = dheight/rect().height ();
    } else if(mScaleType == Uniform) {
        sx = ((dwidth/rect().width ())) * ((dheight/rect().height ()));
        sy = sx;
    } else if(mScaleType == OnlyX){
        sx = dwidth/rect().width ();
        sy = 1;
    } else if(mScaleType == OnlyY){
        sx = 1;
        sy = dheight/rect().height ();
    }

    QMatrix mat = matrix().translate (dx,dy)
            .scale(sx,sy)
            .translate(-dx,-dy);



    mRectPoly = mat.map(mRectPoly);
    prepareGeometryChange ();


}

void BoxCollider::setColliderRotation(QPointF pos, QPointF rotationPoint)
{
    QVector2D vec(pos-rotationPoint);
    qreal dx = rotationPoint.x();
    qreal dy = rotationPoint.y();

    float ang = angleBetweenVectors(vec,mRotationOrigin);
    mRotation += ang;
    mRotationSpanAngle -= ang;

    QMatrix mat = matrix().translate (dx,dy)
            .rotate(ang)
            .translate(-dx,-dy);;

    mRectPoly = mat.map(mRectPoly);
    prepareGeometryChange ();

    mRotationOrigin = vec;
}

void BoxCollider::setFixedRotation(float rot)
{
    QMatrix mat = matrix().translate (rect().center().x(),rect().center().y())
            .rotate(rot)
            .translate(-rect().center().x(), -rect().center().y());

    QPolygonF tmpPoly;
    for(int i=0; i < mRectPoly.count();i++){
        tmpPoly.append(mRectPoly.at(i) * mat);
    }
    prepareGeometryChange ();
    mRectPoly = tmpPoly;
}


float BoxCollider::getMRotation() const
{
    return mRotation;
}

void BoxCollider::setMRotation(float value)
{
    mRotation = value;
}


float BoxCollider::getWidth()
{
    return sqrt(pow(mRectPoly.at(1).x()-mRectPoly.at(0).x(),2)+ pow(mRectPoly.at(1).y()-mRectPoly.at(0).y(),2));
}

qreal BoxCollider::getHeight()
{
    return sqrt(pow(mRectPoly.at(2).x()-mRectPoly.at(1).x(),2)+ pow(mRectPoly.at(2).y()-mRectPoly.at(1).y(),2));
}

QRectF BoxCollider::rect() const
{
    return mRectPoly.boundingRect();;
}

QPolygonF BoxCollider::getRectToDraw()
{   QPolygonF polyToDraw;
    foreach(QPointF p, mRectPoly){
        polyToDraw.append(p+this->pos());
    }
    return polyToDraw;
}

void BoxCollider::setScaleEnabled(bool enable)
{
    mScaleEnabled = enable;
}



void BoxCollider::setRect(const QRectF &rect)
{
    mRect = rect;
    QPolygonF tmp;
    tmp.append(mRect.topLeft());
    tmp.append(mRect.topRight());
    tmp.append(mRect.bottomRight());
    tmp.append(mRect.bottomLeft());
    mRectPoly = tmp;

}

void BoxCollider::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

    Collider::hoverMoveEvent(event);
    setCursor (Qt::ArrowCursor);
    if(mScaleEnabled){
        QVector2D vec(event->pos());
        getScaleOrigin (vec, rect());
    }
}





void BoxCollider::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(mScaleEnabled){

        mScaleOrigin = getScaleOrigin (QVector2D(event->pos ()), rect());
        mItemDragged = mIsValidScaleOriginPoint;
    } else {
        Collider::mousePressEvent (event);
    }
}


void BoxCollider::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(mItemDragged && mScaleEnabled) {
        setNonUniformScale (event->pos(), event->lastPos());
    } else {
        Collider::mouseMoveEvent (event);
    }
}

void BoxCollider::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Collider::mouseReleaseEvent (event);
    if(mScaleEnabled){
        QVector2D vec(event->pos());
        getScaleOrigin (vec, rect());
    }
    mItemDragged = false;
}







