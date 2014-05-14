#ifndef AbstractTreePixmapItem_H
#define AbstractTreePixmapItem_H

#include <QGraphicsPixmapItem>

class AbstractTreePixmapItem : public QGraphicsPixmapItem
{
public:
    AbstractTreePixmapItem(AbstractTreePixmapItem* parent = 0);
    virtual ~AbstractTreePixmapItem();
    int childCount();
    int rowOfChild(AbstractTreePixmapItem *child) const;
    AbstractTreePixmapItem *takeChild(int index);
    AbstractTreePixmapItem *childAt(int index) const;
    AbstractTreePixmapItem *parent() const;

    void addChild(AbstractTreePixmapItem *child);

    AbstractTreePixmapItem *getParent() const;
    void setParent(AbstractTreePixmapItem *value);

    QList<AbstractTreePixmapItem *> getChildren() const;
    AbstractTreePixmapItem *mParent;
    QList<AbstractTreePixmapItem *> mChildren;

private:


};

#endif // AbstractTreePixmapItem_H
