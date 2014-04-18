#ifndef ABSTRACTTREEGRAPHICSITEM_H
#define ABSTRACTTREEGRAPHICSITEM_H

#include "stable_pch.h"

class AbstractTreeGraphicsItem : public QGraphicsItem
{
public:
    AbstractTreeGraphicsItem(AbstractTreeGraphicsItem* parent = 0);
    virtual ~AbstractTreeGraphicsItem();
    int childCount();
    int rowOfChild(AbstractTreeGraphicsItem *child) const;
    AbstractTreeGraphicsItem *takeChild(int index);
    AbstractTreeGraphicsItem *childAt(int index) const;
    AbstractTreeGraphicsItem *parent() const;

    void addChild(AbstractTreeGraphicsItem *child);

    AbstractTreeGraphicsItem *getParent() const;
    void setParent(AbstractTreeGraphicsItem *value);

    virtual QRectF boundingRect() const{ return QRectF();}

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED( painter );
        Q_UNUSED( option );
        Q_UNUSED( widget );
    }

    QList<AbstractTreeGraphicsItem *> getChildren() const;

private:
    AbstractTreeGraphicsItem *mParent;
    QList<AbstractTreeGraphicsItem *> mChildren;

};

#endif // AbstractTreeGraphicsItem_H
