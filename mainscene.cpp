#include "mainscene.h"
#include <QDebug>


MainScene::MainScene()
{
    mRoot = new AbstractTreePixmapItem();
    mRoot->setZValue(2);
    this->addItem(mRoot);
    // set background brush
    QBrush brush;
    brush.setTextureImage(QImage(":/images/checkerboard"));
    QGraphicsRectItem *bg = new QGraphicsRectItem();
    bg->setBrush (brush);
    bg->setFlags(QGraphicsItem::ItemIgnoresTransformations);
    bg->setPen (Qt::NoPen);
    setBackgroundBrush(brush);
    mName = "New Level";
    mShowColliders = true;
    mRotate =false;
    mScale = false;
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

void MainScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView *view =views().front();
    float scalefact = view->transform().m11();
   // QPointF trans = QPointF(view->transform().m31(),view->transform().m32);
    //QRectF theRect = sceneRect();//QRectF(-view->frameSize().width()/2,
//                              -view->frameSize().height()/2
//                              ,view->frameSize().width(),
//                              view->frameSize().height());
   QBrush brush;
    brush.setTextureImage(QImage(":/images/checkerboard"));

       QRectF bgrect = rect;
       //scale the rectangle so that it actually covers the area it's supposed to
       bgrect.setTopLeft(bgrect.topLeft() * scalefact);

       bgrect.setBottomRight(bgrect.bottomRight() * scalefact);

       painter->scale(1/scalefact, 1/scalefact);
       painter->fillRect(bgrect, brush);
       painter->scale(scalefact, scalefact);
      // painter->translate(trans);
}
QString MainScene::getName() const
{
    return mName;
}

void MainScene::setName(const QString &value)
{
    mName = value;
}

bool MainScene::showColliders()
{
    return mShowColliders;
}

void MainScene::setShowColliders(bool set)
{
    mShowColliders = set;
}

bool MainScene::rotate()
{
    return mRotate;
}

void MainScene::setRotate(bool set)
{
    mRotate = set;
}

bool MainScene::scale()
{
    return mScale;
}

void MainScene::setScale(bool set)
{
    mScale = set;
}


