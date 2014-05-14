#include "mainscene.h"
#include <QDebug>


MainScene::MainScene()
{
    mRoot = new AbstractTreePixmapItem;
    mRoot->setZValue(2);
    this->addItem(mRoot);
    // set background brush
    QBrush brush;
    brush.setTextureImage(QImage(":/images/checkerboard"));
    QGraphicsRectItem *bg = new QGraphicsRectItem();
    bg->setBrush (brush);
    bg->setPen (Qt::NoPen);
    setBackgroundBrush(brush);
    mName = "New Level";
}


void MainScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
    QGraphicsScene::dragEnterEvent(event);
}

void MainScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/vnd.text.list")){
        event->accept();
        emit onRequestTemplates(event->scenePos());
    }else{
        QGraphicsScene::dropEvent(event);
    }
}

void MainScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/vnd.text.list")){
        event->accept();
    }else{

        QGraphicsScene::dragMoveEvent(event);
    }
}

void MainScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
    QGraphicsScene::dragEnterEvent(event);
}
AbstractTreePixmapItem* MainScene::getRoot() const
{
    return mRoot;
}

void MainScene::setRoot(AbstractTreePixmapItem *value)
{
    mRoot = value;
}

void MainScene::updatePos()
{
    emit onUpdatePos();
}

void MainScene::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Delete){
        emit onItemDeleted(this->selectedItems());
    }


    QGraphicsScene::keyPressEvent(event);
}
QString MainScene::getName() const
{
    return mName;
}

void MainScene::setName(const QString &value)
{
    mName = value;
}


