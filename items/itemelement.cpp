#include "itemelement.h"



ItemElement::ItemElement(ItemTemplate* tmp)
{
    mTemplate = tmp;
    setPixmap(*tmp->pixmap());
    mName = tmp->name();
    setFlags (ItemIsSelectable | ItemIsMovable |ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);
    showColliders =true;
     mColliderRect = this->pixmap().rect();
}

ItemElement::ItemElement(ItemElement* element)
{
    mTemplate = element->mTemplate;
    setPixmap(*mTemplate->pixmap());
    mName = mTemplate->name();
    setFlags (ItemIsSelectable | ItemIsMovable |ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);
    showColliders =true;
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

QVariant ItemElement::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{

    if (change == ItemPositionChange) {
        if(qobject_cast<MainScene*>(scene())&& scene()->hasFocus()){
            qobject_cast<MainScene*>(scene())->updatePos();
        }
    }
    return QGraphicsPixmapItem::itemChange(change, value);

}

void ItemElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QGraphicsPixmapItem::paint(painter,option,widget);
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
           updateColliderRect(tmpRect);

       }
    }
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

