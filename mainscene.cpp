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

//    connect(this, SIGNAL(selectionChanged()),
//            this, SLOT(handleSelectionChanged()));
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
    } else {
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
    QBrush brush;
    brush.setTextureImage(QImage(":/images/checkerboard"));

    QRectF bgrect = rect;
    bgrect.setTopLeft(bgrect.topLeft() * scalefact);

    bgrect.setBottomRight(bgrect.bottomRight() * scalefact);

    painter->scale(1/scalefact, 1/scalefact);
    painter->fillRect(bgrect, brush);
    painter->scale(scalefact, scalefact);
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

void MainScene::handleSelectionChanged()
{

}

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if(selectedItems().count() == 1 && mDrag){
        QGraphicsItem *i = selectedItems().front();
        i->setPos(int(event->scenePos().x()/50)*50,int(event->scenePos().y()/50)*50);
    } else if(selectedItems().count() > 1 && mDrag){

        int minX =100000;
        int minY =100000;
        int maxX = -100000;
        int maxY = -100000;

        if(selectedItems().count() > 1) {
            foreach(QGraphicsItem* i, selectedItems()){
                if(i->x() < minX){
                    minX = i->pos().x();
                }

                if(i->pos().y() < minY){
                    minY = i->pos().y();
                }

                if(i->pos().x() > maxX){
                    maxX = i->pos().x();
                }

                if(i->pos().y() > maxY){
                    maxY = i->pos().y();
                }
            }
        }

        int x = (int(event->scenePos().x()/50)*50) - minX;
        int y = (int(event->scenePos().y()/50)*50) - minY;

        foreach(QGraphicsItem* i, selectedItems()){
            i->setPos(i->pos().x()+x,i->pos().y()+y);
        }

    } else {
        QGraphicsScene::mouseMoveEvent(event);
    }
}


void MainScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if(selectedItems().count() > 0){
        mDrag =true;
    }
}

void MainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    mDrag =false;
}
