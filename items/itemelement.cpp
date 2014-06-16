#include "itemelement.h"

ItemElement::ItemElement()
{
    mName = "Unamed layer";
    this->setZValue(3);
    mType = LAYER;
    mTemplate = NULL;
    icon.addPixmap(QPixmap(":/icons/layer_icon"));
}

ItemElement::ItemElement(ItemTemplate* tmp)
{
    mType = NORMAL;
    mTemplate = tmp;
    setPixmap(*tmp->pixmap());
    icon = tmp->icon();
    mName = tmp->name();
    setFlags (ItemIsSelectable | ItemIsMovable |ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);
    showColliders =false;
    mRotateEnabled = false;
    mScaleEnabled =false;
    mColliderRect = this->getRect();
    setAcceptHoverEvents (true);
    mScaleFeedbackRectsSize = 6;
}

ItemElement::ItemElement(ItemElement* element)
{
    mType = NORMAL;
    mTemplate = element->mTemplate;
    setPixmap(*mTemplate->pixmap());
    icon = mTemplate->icon();
    mName = mTemplate->name();
    setFlags (ItemIsSelectable | ItemIsMovable |ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);
    showColliders =false;
    mRotateEnabled = false;
    mScaleEnabled = false;
    mColliderRect = this->pixmap().rect();
    setAcceptHoverEvents (true);
    mScaleFeedbackRectsSize = 6;
}
ItemTemplate *ItemElement::getTemplate() const
{
    return mTemplate;
}

void ItemElement::setTemplate(ItemTemplate *value)
{
    mTemplate = value;
}

QRectF ItemElement::getRect()
{
    return AbstractTreePixmapItem::boundingRect();
}

ItemElement::ElementType ItemElement::getType() const
{
    return mType;
}

void ItemElement::setType(const ElementType &value)
{
    mType = value;
}

QString ItemElement::getName() const
{
    return mName;
}

void ItemElement::setName(const QString &value)
{
    mName = value;
}

QRectF ItemElement::boundingRect() const
{

    if(showColliders || mRotateEnabled){
        return QRectF(mColliderRect.topLeft(), mColliderRect.bottomRight());
    } else {
        return AbstractTreePixmapItem::boundingRect();
    }
}

void ItemElement::updateColliderRect(QRectF tmp)
{
    //prepareGeometryChange();
    //update();
    mColliderRect = AbstractTreePixmapItem::boundingRect();
    mColliderRect = mColliderRect.united(tmp);
}

QVariant ItemElement::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{

    if (change == ItemPositionChange || change == ItemRotationChange || change == ItemTransformChange ) {
        if(qobject_cast<MainScene*>(scene())&& scene()->hasFocus()){
            qobject_cast<MainScene*>(scene())->updatePos();
        }
    }

    if (change == ItemPositionHasChanged){

        if(pos().x() < 0){
            setPos(0,pos().y());
        }
        if(pos().y()<0){
            setPos(pos().x(),0);
        }

    }



    return QGraphicsPixmapItem::itemChange(change, value);

}

void ItemElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    viewScale = scene()->views().front()->transform().m11();
    QRectF tmpRect;
    QGraphicsPixmapItem::paint(painter,option,widget);
    if(mType == LAYER){
        return;
    }
    if(qobject_cast<MainScene*>(scene())){
        showColliders = qobject_cast<MainScene*>(scene())->showColliders();
        if(this->isSelected()){
            mRotateEnabled = qobject_cast<MainScene*>(scene())->rotate();
            mScaleEnabled = qobject_cast<MainScene*>(scene())->scale();
        } else {
            mRotateEnabled = false;
            mScaleEnabled = false;
        }
    } else {
        mRotateEnabled = false;
        mScaleEnabled = false;
    }

    if(qobject_cast<SplitScene*>(scene())){
        showColliders = true;
    }

    if(showColliders){

        QList<QGraphicsItem*> colliders = mTemplate->getColliderRoot()->childItems();


        foreach(QGraphicsItem* col, colliders){
            if(col->type() == QGraphicsItem::UserType+1){

                BoxCollider* boxCol = static_cast<BoxCollider*>(col);
                tmpRect = tmpRect.united(boxCol->getRectToDraw().boundingRect());

                QPen pen(QColor(Qt::cyan));
                pen.setWidth (0);
                painter->setPen (pen);

                painter->drawPolygon(boxCol->getRectToDraw());

                painter->setBrush (QColor(Qt::green));
                painter->setOpacity (0.3);
                painter->drawPolygon(boxCol->getRectToDraw());
            }
            if(col->type() == QGraphicsItem::UserType+2){

                MeshCollider* meshCol = static_cast<MeshCollider*>(col);
                tmpRect = tmpRect.united(meshCol->getPolyToDraw().boundingRect());

                QPen pen(QColor(Qt::cyan));
                pen.setWidth (0);
                painter->setPen (pen);

                painter->drawPolygon(meshCol->getPolyToDraw());

                painter->setBrush (QColor(Qt::green));
                painter->setOpacity (0.3);
                painter->drawPolygon(meshCol->getPolyToDraw());
            }
            if(col->type() == QGraphicsItem::UserType+3){

                CircleCollider* cirCol = static_cast<CircleCollider*>(col);
                int rad = (int)cirCol->getRadius();
                QPointF center = cirCol->getCenter();

                QPointF topLeft = QPointF(center.x() - rad,center.y() - rad);
                QPointF bottomRight = QPointF((center.x() + rad),(center.y() + rad));

                tmpRect = tmpRect.united(QRectF(topLeft,bottomRight));

                QPen pen(QColor(Qt::cyan));
                pen.setWidth (0);
                painter->setPen (pen);

                painter->drawEllipse(center,rad,rad);

                painter->setBrush (QColor(Qt::green));
                painter->setOpacity (0.3);
                painter->drawEllipse(center,rad,rad);
            }
        }
    }
    QPen pen(QColor(Qt::cyan));
    if(mRotateEnabled){


        QPen pen(QColor(Qt::cyan));
        pen.setWidth (0);
        //pen.setCosmetic(false);
        painter->setPen (pen);
        painter->setBrush (QColor(Qt::green));
        painter->setOpacity (0.3);
        float rad = 50/(transform().m11()*viewScale);

        QPointF topLeft = QPointF(getRect().center().x() - rad,getRect().center().y() - rad);
        QPointF bottomRight = QPointF((getRect().center().x() + rad),(getRect().center().y() + rad));

        tmpRect = tmpRect.united(QRectF(topLeft,bottomRight));


        painter->drawEllipse(getRect().center(),rad,rad);
        if(mItemDragged){
            pen.setColor(QColor(Qt::darkGreen));
            painter->setPen(pen);
            painter->setBrush(QColor(Qt::darkGreen));
            painter->drawPie(QRectF(getRect().center()-QPointF(rad,-rad),
                                    getRect().center()+QPointF(rad,-rad)),
                             mRotationStartAngle*16,
                             mRotationSpanAngle*16);
        }
    }
    if(mScaleEnabled){
        drawScaleOverlay(painter,pen,pixmap().rect());
    }
   updateColliderRect(tmpRect);
}

void ItemElement::drawScaleOverlay(QPainter *painter, QPen pen, QRectF outlineRect)
{
    painter->setBrush (Qt::NoBrush);
    painter->setOpacity (0.9);



    pen.setStyle (Qt::SolidLine);
    // draw rect bounds
    painter->drawRect (outlineRect);
    painter->setBrush (Qt::NoBrush);
    float cSize = mScaleFeedbackRectsSize/(transform().m11()*viewScale);
    //Corners

    QVector<QRectF> rects;

    rects  << QRectF( outlineRect.bottomRight ().x()-(outlineRect.width()/2)-(cSize/2), outlineRect.bottomRight ().y()-(cSize/2),cSize,cSize)
           << QRectF( outlineRect.bottomRight ().x()-(outlineRect.width()/2)-(cSize/2), outlineRect.topRight().y()-(cSize/2),cSize,cSize)
           << QRectF( outlineRect.bottomLeft ().x()-(cSize/2), outlineRect.bottomRight ().y()-(outlineRect.height()/2)-(cSize/2),cSize,cSize)
           << QRectF( outlineRect.bottomRight().x()-(cSize/2), outlineRect.bottomRight ().y()-(outlineRect.height()/2)-(cSize/2),cSize,cSize);

    painter->setBrush (QColor(Qt::blue));
    pen.setStyle (Qt::SolidLine);

    painter->drawRects (rects);
}

void ItemElement::setDraggedRotation(QPointF pos, QPointF lastPos)
{
    QVector2D vec(pos-mRotationCenter);
    QVector2D lastVec(lastPos-mRotationCenter);

    float ang = angleBetweenVectors(vec, lastVec);
    mRotationSpanAngle -= ang;
    setRotation(rotation() + ang);
}

float ItemElement::angleBetweenVectors(QVector2D v1, QVector2D v2)
{
    float dot = QVector2D::dotProduct(v1,v2);
    float det = v1.x ()*v2.y() - v1.y ()*v2.x();

    return (-qRadiansToDegrees(atan2(det,dot)));
}

void ItemElement::setNonUniformScale(QPointF pos, QPointF lastPos)
{
    Q_UNUSED(lastPos);

    qreal dx, dy;
    qreal s;
    dx = mScaleOrigin.x();
    dy = mScaleOrigin.y();




    float dwidth = abs(pos.x()-dx);
    float dheight = abs(pos.y()-dy);

    QPointF posMaped = mapToScene(pos);
    QPointF xyMaped = mapToScene(QPointF(dx,dy));

    if((mScaleXDirection == PosXAxis &&  posMaped.x() > xyMaped.x()) || (mScaleXDirection == PosXAxis && dwidth == 0)){
        dwidth =-2;
        mScaleXDirection = NegXAxis;
    } else if((mScaleXDirection == NegXAxis && posMaped.x() < xyMaped.x()) || (mScaleXDirection == NegXAxis && dwidth == 0)) {
        dwidth = -2;
        mScaleXDirection = PosXAxis;
    }
    if((mScaleYDirection == PosYAxis && posMaped.y() >xyMaped.y()) || (mScaleYDirection == PosYAxis && dheight == 0)){
        dheight =-2;
        mScaleYDirection = NegYAxis;
    } else if((mScaleYDirection == NegYAxis && posMaped.y() < xyMaped.y()) || (mScaleYDirection == NegYAxis && dheight == 0)) {
        dheight = -2;
        mScaleYDirection = PosYAxis;
    }

    if(mScaleType == x){



        s = dwidth/boundingRect().width();


    } else if(mScaleType == y){

        s = dheight/boundingRect().height();

    }



    QTransform trans = transform().translate (dx,dy)
            .scale(s,s)
            .translate(-dx,-dy);


    setTransform(trans);



}

QPointF ItemElement::getScaleOrigin(QVector2D vec, QRectF rect)
{
    mIsValidScaleOriginPoint = true;
    if(vec.distanceToPoint (QVector2D(QPointF(rect.bottomRight ().x()-(rect.width()/2),rect.bottomRight ().y()))) < 10) {
        QPointF dirTop = mapToScene(rect.topLeft ());
        QPointF dirBottom = mapToScene(rect.bottomRight ());
        if(dirBottom.y() > dirTop.y()){

            mScaleYDirection = NegYAxis;
        } else{
            mScaleYDirection = PosYAxis;
        }
        mScaleType = y;
        setCursor (Qt::SizeVerCursor);
        return (QPointF(pixmap().rect().topRight ().x()-(pixmap().rect().width()/2),pixmap().rect().topRight ().y()));
    } else if(vec.distanceToPoint (QVector2D(QPointF(rect.topRight ().x()-(rect.width()/2),rect.topRight ().y()))) < 10) {
        QPointF dirTop = mapToScene(rect.topLeft ());
        QPointF dirBottom = mapToScene(rect.bottomRight ());
        if(dirBottom.y() > dirTop.y()){
            mScaleYDirection = PosYAxis;
        } else{
            mScaleYDirection = NegYAxis;
        }

        mScaleType = y;
        setCursor (Qt::SizeVerCursor);
        return (QPointF(pixmap().rect().bottomRight ().x()-(pixmap().rect().width()/2),pixmap().rect().bottomRight ().y()));
    } else if(vec.distanceToPoint (QVector2D(QPointF(rect.bottomRight ().x(),rect.bottomRight ().y()-(rect.height()/2)))) < 10) {
        QPointF dirLeft = mapToScene(rect.bottomLeft ());
        QPointF dirRight = mapToScene(rect.bottomRight ());
        if(dirRight.x() > dirLeft.x()){
             mScaleXDirection = NegXAxis;
        } else{
           mScaleXDirection = PosXAxis;
        }
        mScaleType = x;
        setCursor (Qt::SizeHorCursor);
        return (QPointF(pixmap().rect().bottomLeft ().x(),pixmap().rect().bottomLeft ().y()-(pixmap().rect().height()/2)));
    } else if(vec.distanceToPoint (QVector2D(QPointF(rect.bottomLeft ().x(),rect.bottomLeft ().y()-(rect.height()/2)))) < 10) {
        QPointF dirLeft = mapToScene(rect.bottomLeft ());
        QPointF dirRight = mapToScene(rect.bottomRight ());
        if(dirRight.x() > dirLeft.x()){
            mScaleXDirection = PosXAxis;
        } else{
            mScaleXDirection = NegXAxis;
        }
        mScaleType = x;
        setCursor (Qt::SizeHorCursor);
        return (QPointF(pixmap().rect().bottomRight ().x(),pixmap().rect().bottomRight ().y()-(pixmap().rect().height()/2))) ;
        //return (QPointF(17,12));
    }
    mIsValidScaleOriginPoint = false;
    setCursor (Qt::ArrowCursor);
    return QPointF();
}

void ItemElement::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

    AbstractTreePixmapItem::hoverMoveEvent(event);
    setCursor (Qt::ArrowCursor);
    if(mScaleEnabled){
        QVector2D vec(QPointF(event->pos().x()*(transform().m11()*viewScale),event->pos().y()*(transform().m11()*viewScale)));
        getScaleOrigin (vec, QRectF(pixmap().rect().topLeft()*(transform().m11()*viewScale),pixmap().rect().bottomRight()*(transform().m11()*viewScale)));
    }
}
void ItemElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(qobject_cast<MainScene*>(scene())){
        mRotateEnabled = qobject_cast<MainScene*>(scene())->rotate();
        mScaleEnabled = qobject_cast<MainScene*>(scene())->scale();
    } else {
        mRotateEnabled = false;
        mScaleEnabled = false;
    }
    if(mScaleEnabled){
        scene()->clearSelection();
        setSelected(true);
        QVector2D vec(QPointF(event->pos().x()*(transform().m11()*viewScale),event->pos().y()*(transform().m11()*viewScale)));
        mScaleOrigin = getScaleOrigin (vec, QRectF(pixmap().rect().topLeft()*(transform().m11()*viewScale),pixmap().rect().bottomRight()*(transform().m11()*viewScale)));

        mItemDragged = mIsValidScaleOriginPoint;
    } else  if(mRotateEnabled){
        scene()->clearSelection();
        setSelected(true);
        setTransformOriginPoint(pixmap().rect().center());
        mRotationCenter = pixmap().rect().center();
        mItemDragged = true;
        mRotationStartAngle = angleBetweenVectors(QVector2D(1,0), QVector2D(event->pos()-mRotationCenter));
        mRotationSpanAngle = 0;
    } else {
        QGraphicsItem::mousePressEvent (event);
    }
}


void ItemElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(mItemDragged && mScaleEnabled) {
        setNonUniformScale (event->pos(), event->lastPos());
    } else if(mItemDragged && mRotateEnabled) {
        setDraggedRotation(event->pos(),event->lastPos());
    } else {

        AbstractTreePixmapItem::mouseMoveEvent (event);
    }


}

void ItemElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent (event);
    mItemDragged = false;
    if(mScaleEnabled){
        QVector2D vec(event->pos());
        getScaleOrigin (vec, QRectF(pixmap().rect().topLeft()*(transform().m11()*viewScale),pixmap().rect().bottomRight()*(transform().m11()*viewScale)));
    }
    mItemDragged = false;
}




