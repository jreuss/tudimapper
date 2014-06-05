#include "itemelement.h"



ItemElement::ItemElement(ItemTemplate* tmp)
{
    mTemplate = tmp;
    setPixmap(*tmp->pixmap());
    mName = tmp->name();
    setFlags (ItemIsSelectable | ItemIsMovable |ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);
     showColliders =false;
     mRotateEnabled = false;
     mColliderRect = this->pixmap().rect();
}

ItemElement::ItemElement(ItemElement* element)
{
    mTemplate = element->mTemplate;
    setPixmap(*mTemplate->pixmap());
    mName = mTemplate->name();
    setFlags (ItemIsSelectable | ItemIsMovable |ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);
    showColliders =false;
    mRotateEnabled = false;
    mColliderRect = this->pixmap().rect();
}
ItemTemplate *ItemElement::getTemplate() const
{
    return mTemplate;
}

void ItemElement::setTemplate(ItemTemplate *value)
{
    mTemplate = value;
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

    if(showColliders){
        return QRectF(mColliderRect.topLeft(), mColliderRect.bottomRight());
    } else {
        return this->pixmap().rect();
    }
}

void ItemElement::updateColliderRect(QRectF tmp)
{
  mColliderRect = this->pixmap().rect();
  mColliderRect = mColliderRect.united(tmp);
}

QVariant ItemElement::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{

    if (change == ItemPositionChange || change == ItemRotationChange ) {
        if(qobject_cast<MainScene*>(scene())&& scene()->hasFocus()){
            qobject_cast<MainScene*>(scene())->updatePos();
        }
    }



    return QGraphicsPixmapItem::itemChange(change, value);

}

void ItemElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QGraphicsPixmapItem::paint(painter,option,widget);
    if(qobject_cast<MainScene*>(scene())){
        showColliders = qobject_cast<MainScene*>(scene())->showColliders();
        if(this->isSelected()){
           mRotateEnabled = qobject_cast<MainScene*>(scene())->rotate();
        }
    } else {
        mRotateEnabled = false;
     }

    if(qobject_cast<SplitScene*>(scene())){
        showColliders = true;
    }

    if(showColliders){
        QRectF tmpRect;
       QList<QGraphicsItem*> colliders = mTemplate->getColliderRoot()->childItems();


       foreach(QGraphicsItem* col, colliders){
           if(col->type() == QGraphicsItem::UserType+1){

               BoxCollider* boxCol = static_cast<BoxCollider*>(col);
               tmpRect = tmpRect.united(boxCol->getRectToDraw().boundingRect());

               QPen pen(QColor(Qt::cyan));
               pen.setWidth (1);
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
               pen.setWidth (1);
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
               pen.setWidth (1);
               painter->setPen (pen);

               painter->drawEllipse(center,rad,rad);

               painter->setBrush (QColor(Qt::green));
               painter->setOpacity (0.3);
               painter->drawEllipse(center,rad,rad);
           }


           updateColliderRect(tmpRect);

       }
       if(mRotateEnabled){
          painter->drawEllipse(boundingRect().center(),50,50);
          if(mItemDragged){
              painter->setPen(QColor(Qt::darkGreen));
              painter->setBrush(QColor(Qt::darkGreen));
             // painter->drawPie(QRectF(boundingRect().center()-QPointF(50,-50),boundingRect().center()+QPointF(50,-50)),mRotationStartAngle*16,mRotationSpanAngle*16);
          }
       }
    }
}

void ItemElement::setDraggedRotation(QPointF pos, QPointF lastPos)
{
    QVector2D vec(pos-boundingRect().center());
    QVector2D lastVec(lastPos-boundingRect().center());

    float ang = angleBetweenVectors(vec, lastVec);

    setRotation(rotation() + ang);
}

float ItemElement::angleBetweenVectors(QVector2D v1, QVector2D v2)
{
    float dot = QVector2D::dotProduct(v1,v2);
    float det = v1.x ()*v2.y() - v1.y ()*v2.x();

    return (-qRadiansToDegrees(atan2(det,dot)));
}

void ItemElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    if(mScaleEnabled){

//        mScaleOrigin = getScaleOrigin (QVector2D(event->pos ()), rect());
//        mItemDragged = mIsValidScaleOriginPoint;
    if(mRotateEnabled){
        mItemDragged = true;
    } else {
       AbstractTreePixmapItem::mousePressEvent (event);
    }
}


void ItemElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    if(mItemDragged && mScaleEnabled) {
//        setNonUniformScale (event->pos(), event->lastPos());
    if(mItemDragged && mRotateEnabled) {
        setTransformOriginPoint(pixmap().rect().center());
        setDraggedRotation(event->pos(),event->lastPos());
    } else {
          AbstractTreePixmapItem::mouseMoveEvent (event);
    }
}

void ItemElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
      AbstractTreePixmapItem::mouseReleaseEvent (event);
//    if(mScaleEnabled){
//        QVector2D vec(event->pos());
//        getScaleOrigin (vec, rect());
//    }
    mItemDragged = false;
}




