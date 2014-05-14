#ifndef ITEMELEMENT_H
#define ITEMELEMENT_H
#include <items/abstracttreepixmapitem.h>
#include <items/itemtemplate.h>
#include <mainscene.h>
#include <QDebug>

class ItemElement : public AbstractTreePixmapItem
{

public:
    ItemElement(ItemTemplate* tmp);
    ItemElement(ItemElement* element);
    ItemTemplate* mTemplate;

    QString getName() const;
    void setName(const QString &value);

    ItemTemplate *getTemplate() const;
    void setTemplate(ItemTemplate *value);

private:
    QString mName;
signals:

    // QGraphicsItem interface
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // ITEMELEMENT_H
