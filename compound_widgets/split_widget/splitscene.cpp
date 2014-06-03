#include "splitscene.h"

#include "items/itemelement.h"



SplitScene::SplitScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void SplitScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    ItemElement *item = dynamic_cast<ItemElement*>(items(event->scenePos()).first());
    if(item){
        emit onDoubleClick(item->getTemplate());
    }
}
