#include "meshcollider.h"

MeshCollider::MeshCollider(const QString &name ,bool consolidated)
{
    mConsolidated = consolidated;
    mScaleEnabled = false;
    mRotateEnabled = false;
    mCanBeDeleted = false;
    mIsTrigger = false;
    mName = name;


    setFlags (ItemIsSelectable | ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);

    setAcceptHoverEvents (true);

}

MeshCollider::~MeshCollider()
{
    qDeleteAll(mChildNodes);
}


QRectF MeshCollider::boundingRect() const
{
    if(!mConsolidated){
        return mRect;
    } else {
        QRectF tmpRect = mPoly.boundingRect();
        return QRectF(QPointF(tmpRect.topLeft().x()-mScaleFeedbackRectsSize,
                             tmpRect.topLeft().y()-mScaleFeedbackRectsSize),
                      QPointF(tmpRect.bottomRight().x()+mScaleFeedbackRectsSize,
                               tmpRect.bottomRight().y()+mScaleFeedbackRectsSize));
    }
}

void MeshCollider::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *widget __attribute__((unused)))
{
    QPen pen(QColor(Qt::cyan));
    pen.setWidth (1);
    painter->setPen (pen);

    if(!mConsolidated){
        drawNonConsolidatedMesh(painter, pen);
    } else {
        drawPoly(painter, option, pen);

        if(mScaleEnabled) {
            drawScaleOverlay (painter, pen, mPoly.boundingRect());
        }

        if(mRotateEnabled && mItemDragged){
            drawRotateOverlay(painter,pen);
        }
    }
}

void MeshCollider::drawRotateOverlay(QPainter *painter, QPen pen)
{
    Q_UNUSED(pen);

    painter->drawLine(mRotationCenter,mRotationOriginPoint);
    painter->drawEllipse(mRotationCenter,15,15);
}

void MeshCollider::drawPoly(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QPen pen)
{
    if(option->state & QStyle::State_Selected){
        pen.setColor (QColor(Qt::blue));
        pen.setWidth (2);
    }

    painter->setPen (pen);
    painter->drawPolygon(mPoly);

    painter->setBrush (QColor(Qt::green));
    painter->setOpacity (0.3);
    painter->drawPolygon (mPoly);
}

void MeshCollider::drawNonConsolidatedMesh(QPainter *painter, QPen pen)
{
    if(childCount() > 1) {
        for(int i=0; i < childCount ()-1; ++i) {
            painter->drawLine(mChildNodes[i]->pos (), mChildNodes[i+1]->pos());
        }

        if(childCount () > 2) {
            pen.setColor (QColor(Qt::red));
            pen.setStyle (Qt::DotLine);
            painter->setPen (pen);

            painter->drawLine (mChildNodes.back()->pos(),
                               mChildNodes.front ()->pos());
        }
    }
}

void MeshCollider::setScaleEnabled(bool enable)
{
    mScaleEnabled = enable;
}


QPainterPath MeshCollider::shape() const
{
    QPainterPath path;

    if(!mConsolidated || mScaleEnabled){
        path.addRect (mRect);
    } else {
        path.addPolygon (mPoly);
    }
    return path;
}


void MeshCollider::setSize(const QSizeF &size)
{
    mRect.setSize (size);
}

void MeshCollider::addChildNode(MeshNode *child)
{
   if(child) {
        child->setParentItem (this);
        mChildNodes.append (child);
   }
}

void MeshCollider::addChildrenNodes(const QList<MeshNode *> &children)
{
    foreach(MeshNode * c, children) {
        c->setParentItem (this);
        mChildNodes.append (c);
    }
}

void MeshCollider::addChildNodeAt(int index, MeshNode *child)
{
    if(index < mChildNodes.count () && child) {
        child->setParentItem (this);
        mChildNodes.insert (index, child);
    }
}

void MeshCollider::removeChild(MeshNode *child)
{
    int index = mChildNodes.indexOf (child);
    if(index > -1) {
        delete mChildNodes.takeAt (index);
    }
}

void MeshCollider::clearChildren()
{
    qDeleteAll(mChildNodes);
    mChildNodes.clear ();
}

void MeshCollider::setNonUniformScale(QPointF pos, QPointF lastPos)
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
    } else if((mScaleYDirection == NegYAxis && pos.y() < dy )|| (mScaleYDirection == NegYAxis && dheight == 0)) {
        dheight = -2;
        mScaleYDirection = PosYAxis;
    }
    if(mScaleType== BothXY){
       sx = dwidth/mPoly.boundingRect ().width ();
       sy = dheight/mPoly.boundingRect ().height ();
    } else if(mScaleType == Uniform) {
              sx = ((dwidth/mPoly.boundingRect ().width ())) * ((dheight/mPoly.boundingRect ().height ()));
              sy = sx;
    } else if(mScaleType == OnlyX){
        sx = dwidth/mPoly.boundingRect ().width ();
        sy = 1;
    } else if(mScaleType == OnlyY){
            sx = 1;
            sy = dheight/mPoly.boundingRect ().height ();;
        }

    QMatrix mat = matrix().translate (dx,dy)
            .scale(sx,sy)
            .translate(-dx,-dy);

    foreach(MeshNode *m, mChildNodes)
    {
        m->setPos (m->pos ()*mat);
    }
    updatePolygon();

}

void MeshCollider::setColliderRotation(QPointF pos,  QPointF rotationPoint)
{
     QVector2D vec(pos-rotationPoint);
        qreal dx = rotationPoint.x();
        qreal dy = rotationPoint.y();

        float dot = QVector2D::dotProduct(vec,mRotationOrigin);
        float det = vec.x ()*mRotationOrigin.y() - vec.y ()*mRotationOrigin.x();

        float ang = -qRadiansToDegrees(atan2(det,dot));
        QMatrix mat = matrix().translate (dx,dy)
                .rotate(ang)
                .translate(-dx,-dy);

        foreach(MeshNode *m, mChildNodes)
        {
            m->setPos (m->pos ()*mat);
        }
        mRotationOrigin = vec;
}

void MeshCollider::setCanBeDeleted(bool can)
{
    mCanBeDeleted = can;
}

bool MeshCollider::canBeDeleted()
{
    return mCanBeDeleted;
}

QPolygonF MeshCollider::getPolyToDraw()
{
    return mPoly;
}

int MeshCollider::indexOfChild(MeshNode *child)
{
    return mChildNodes.indexOf (child);
}

int MeshCollider::childCount() const
{
    return mChildNodes.count ();
}

void MeshCollider::childCalledConsolidate()
{
    mConsolidated = true;
}

QVariant MeshCollider::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedHasChanged) {
    } else if(change == ItemScaleHasChanged) {
         updatePolygon ();
    } else if(change == ItemRotationChange) {
        //QPolygonF poly = updatePolygon ();
        setTransformOriginPoint (boundingRect ().center());
    }

    return QGraphicsItem::itemChange(change, value);
}


int MeshCollider::type() const
{
    return QGraphicsItem::UserType +2;
}

void MeshCollider::updatePolygon()
{
    QPolygonF poly;

    prepareGeometryChange ();
    for(int m = 0; m < mChildNodes.count (); m++) {
        poly.append (mChildNodes[m]->pos());
    }

    mPoly = poly;
}



void MeshCollider::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

    Collider::hoverMoveEvent(event);
    setCursor (Qt::ArrowCursor);
    if(mScaleEnabled){
        QVector2D vec(event->pos());
        getScaleOrigin (vec, mPoly.boundingRect());
    }
}





void MeshCollider::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!mConsolidated){
        MeshNode *node = new MeshNode(this);
        node->setPos (event->pos ());
        addChildNode (node);
    } else if(mScaleEnabled){
        mScaleOrigin = getScaleOrigin (QVector2D(event->pos ()), mPoly.boundingRect());
        mItemDragged = mIsValidScaleOriginPoint;
    } else if(mRotateEnabled){
        mRotationOrigin = QVector2D(event->pos () - boundingRect ().center ());
        mRotationCenter = boundingRect().center();
        mItemDragged = true;
    }
    else {
        Collider::mousePressEvent (event);
    }
}


void MeshCollider::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(mItemDragged && mScaleEnabled) {
        setNonUniformScale (event->pos(), event->lastPos());
    } else {
        Collider::mouseMoveEvent (event);
    }
}

void MeshCollider::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Collider::mouseReleaseEvent (event);
    updatePolygon ();
    if(mScaleEnabled){
        QVector2D vec(event->pos());
        getScaleOrigin (vec, mPoly.boundingRect());
    }
    mItemDragged = false;
}
