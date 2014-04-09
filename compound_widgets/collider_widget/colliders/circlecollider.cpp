#include "circlecollider.h"

CircleCollider::CircleCollider(const QString &name)
{
    mIsTrigger = false;
    mName = name;
    mRadius = 0;
    mCenter = QPointF(0,0);

    mScaleEnabled = false;
    mRotateEnabled = false;


    setFlags (ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);

    setAcceptHoverEvents (true);
}

QRectF CircleCollider::boundingRect() const
{
    return (QRect(QPoint(mCenter.x()-mRadius, mCenter.y()-mRadius),QPoint(mCenter.x()+mRadius, mCenter.y()+mRadius)));
}

void CircleCollider::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget __attribute__((unused)))
{
    QPen pen(QColor(Qt::cyan));
    pen.setWidth (1);
    painter->setPen (pen);


    drawCircle(painter, option, pen);

    if(mScaleEnabled) {
        drawScaleOverlay (painter, pen, boundingRect());
    }

}


void CircleCollider::drawCircle(QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QPen pen)
{
    if(option->state & QStyle::State_Selected){
        pen.setColor (QColor(Qt::blue));
        pen.setWidth (2);
    }

    painter->setPen (pen);
    painter->drawEllipse(mCenter,(int)mRadius,(int)mRadius);
    painter->setBrush (QColor(Qt::green));
    painter->setOpacity (0.3);
    painter->drawEllipse(mCenter,(int)mRadius,(int)mRadius);
}

QPainterPath CircleCollider::shape() const
{
    QPainterPath path;

    path.addRect (QRect(QPoint(mCenter.x()-mRadius, mCenter.y()-mRadius),QPoint(mCenter.x()+mRadius, mCenter.y()+mRadius)));

    return path;
}

void CircleCollider::setScaleEnabled(bool enable)
{
    mScaleEnabled = enable;
}

void CircleCollider::setNonUniformScale(QPointF pos, QPointF lastPos)
{
    Q_UNUSED(lastPos);

    qreal dx, dy;
    qreal sx, sy;

    dx = mScaleOrigin.x();
    dy = mScaleOrigin.y();

    qreal dwidth = abs(pos.x ()-dx);
    qreal dheight = abs(pos.y()-dy);


    if((mScaleXDirection == PosXAxis && pos.x() > dx)|| (mScaleXDirection == PosXAxis && dwidth == 0)){
        dwidth =-2;;
        //lastDwidth =abs(lastPos.x()-dx);;
        mScaleXDirection = NegXAxis;
    } else if((mScaleXDirection == NegXAxis && pos.x() < dx) || (mScaleXDirection == NegXAxis && dwidth == 0)) {
        dwidth = -2;
        //lastDwidth =abs(lastPos.y()-dy);
        mScaleXDirection = PosXAxis;
    }


    if((mScaleYDirection == PosYAxis && pos.y() > dy) || (mScaleYDirection == PosYAxis && dheight  == 0)){
        dheight =-2;
        mScaleYDirection = NegYAxis;
    } else if((mScaleYDirection == NegYAxis && pos.y() < dy) || (mScaleYDirection == NegYAxis && dheight  == 0)) {

        dheight = -2;
        mScaleYDirection = PosYAxis;
    }

    //qDebug()<< "NewUniform " << newUniform << "Uniform " << mUniformScaleVector;
    if(mScaleType == Uniform || mScaleType == BothXY) {
        //              if(dwidth < 0 && dheight > 0)
        //                  dheight *=-1;

        sx =  (abs(dwidth)/lastDwidth)/2 + (abs(dheight)/lastDheight);
        if(dheight < 0 || dwidth < 0){
            sx =-1;
        }
        qDebug() << "sx " << sx << " dWidht " << dwidth << " LastDWidth " << lastDwidth;
        sy = sx;

    } else if(mScaleType == OnlyX){
        sx = dwidth/lastDwidth;
        sy = sx;
    } else if(mScaleType == OnlyY){
        sy = dheight/lastDheight;
        sx = sy;
    }

    QMatrix mat = matrix().translate (dx,dy)
            .scale(sx,sy)
            .translate(-dx,-dy);

    prepareGeometryChange();
    lastDwidth = abs(dwidth);
    lastDheight = abs(dheight);
    mCenter = mCenter * mat;
    mRadius = mRadius * sx;
    if(mRadius < 0){
        mRadius *=-1;
    }

}


int CircleCollider::type() const
{
    return QGraphicsItem::UserType +3;
}

float CircleCollider::getRadius() const
{
    return mRadius;
}

void CircleCollider::setRadius(float value)
{
    mRadius = value;
}

QPointF CircleCollider::getCenter() const
{
    return mCenter;
}

void CircleCollider::setCenter(const QPointF &value)
{
    mCenter = value;
}

void CircleCollider::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

    Collider::hoverMoveEvent(event);
    setCursor (Qt::ArrowCursor);
    if(mScaleEnabled){
        QVector2D vec(event->pos());
        getScaleOrigin (vec, boundingRect());
    }
}





void CircleCollider::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(mScaleEnabled){
        mScaleOrigin = getScaleOrigin (QVector2D(event->pos ()), boundingRect());
        lastDwidth = abs(event->pos().x()-mScaleOrigin.x());
        lastDheight = abs(event->pos().y()-mScaleOrigin.y());
        mItemDragged = mIsValidScaleOriginPoint;
    } else {
        Collider::mousePressEvent (event);
    }
}


void CircleCollider::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(mItemDragged && mScaleEnabled) {
        setNonUniformScale (event->pos(), event->lastPos());
    } else {
        Collider::mouseMoveEvent (event);
    }
}

void CircleCollider::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Collider::mouseReleaseEvent (event);
    if(mScaleEnabled){
        QVector2D vec(event->pos());
        getScaleOrigin (vec, boundingRect());
    }
    mItemDragged = false;
}




