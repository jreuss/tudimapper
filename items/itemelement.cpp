#include "itemelement.h"


ItemElement::ItemElement(ItemTemplate* tmp)
{
    mTemplate = tmp;
    setPixmap(*tmp->pixmap());
    mName = tmp->name();
    setFlags (ItemIsSelectable | ItemIsMovable |ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);
}

ItemElement::ItemElement(ItemElement* element)
{
    mTemplate = element->mTemplate;
    setPixmap(*mTemplate->pixmap());
    mName = mTemplate->name();
    setFlags (ItemIsSelectable | ItemIsMovable |ItemSendsGeometryChanges |
              ItemSendsScenePositionChanges | ItemIsFocusable);
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

