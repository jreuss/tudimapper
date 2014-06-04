#ifndef ITEMELEMENT_H
#define ITEMELEMENT_H
#include "items/abstracttreepixmapitem.h"
#include "items/itemtemplate.h"
#include "items/abstracttreegraphicsitem.h"
//#include "compound_widgets/collider_widget/colliderwidget.h"
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
    bool showColliders;
    ItemTemplate* mTemplate;
    QRectF mColliderRect;
signals:

    // QGraphicsItem interface
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    void updateColliderRect(QRectF tmp);
};


#endif // ITEMELEMENT_H
