#ifndef SPLITSCENE_H
#define SPLITSCENE_H

class ItemTemplate;

#include <QGraphicsScene>
#include "items/itemtemplate.h"




class SplitScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SplitScene(QObject *parent = 0);

signals:
void onDoubleClick(ItemTemplate*);

public slots:


////    // QGraphicsScene interface
protected:
   void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SPLITSCENE_H
