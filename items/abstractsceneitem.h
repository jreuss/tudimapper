#ifndef ABSTRACTSCENEITEM_H
#define ABSTRACTSCENEITEM_H

#include <QGraphicsScene>

class AbstractSceneItem : public QGraphicsScene
{
    Q_OBJECT
public:
    AbstractSceneItem(AbstractSceneItem* parent = 0);
    virtual ~AbstractSceneItem();
    int childCount();
    int rowOfChild(AbstractSceneItem *child) const;
    AbstractSceneItem *takeChild(int index);
    AbstractSceneItem *childAt(int index) const;
    AbstractSceneItem *parent() const;

    void addChild(AbstractSceneItem *child);

    AbstractSceneItem *getParent() const;
    void setParent(AbstractSceneItem *value);

    QList<AbstractSceneItem *> getChildren() const;
    AbstractSceneItem *mParent;
    QList<AbstractSceneItem *> mChildren;

private:


signals:

public slots:

};

#endif // ABSTRACTSCENEITEM_H
