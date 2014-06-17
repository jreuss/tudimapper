#include "mainscene.h"
#include <QDebug>

MainScene::MainScene()
{
    mRoot = new AbstractTreePixmapItem();
    mRoot->setZValue(2);
    this->addItem(mRoot);
    model = new ElementTreeModel(2);
    model->setRoot(mRoot);
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
    mRotate = false;
    mScale  = false;
    overlay = new QGraphicsRectItem(QRect());
    addItem(overlay);
    overlay->hide();
    copy = false;

    gridX = gridY = 0;

    connect(this, SIGNAL(selectionChanged()),
            this, SLOT(handleSelectionChanged()));
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
    QGraphicsView *view = views().front();
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

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if(copy){

        foreach(QGraphicsItem* i, selectedItems()){
            ItemElement *toCopy = static_cast<ItemElement*>(i);
            if(toCopy->getType() == ItemElement::NORMAL){

                ItemElement *newTmp = new ItemElement(toCopy);
                ItemElement *parent =static_cast<ItemElement*>(toCopy->parentItem());
                newTmp->setParentItem(parent);
                model->insertItem(parent->getChildren().count(), parent, newTmp);
                newTmp->setPos(toCopy->pos());
                toCopy->setSelected(false);
                newTmp->setSelected(true);
            }
        }
    }

    copy = false;

    ZoomedGraphicView *view = static_cast<ZoomedGraphicView*>(views().front());

    if(view->snapToGrid) {
        gridX = view->stepX;
        gridY = view->stepY;
    } else {
        gridX = 1;
        gridY = 1;
    }

    if(selectedItems().count() == 1 && mDrag){
        QGraphicsItem *i = selectedItems().front();
        int x = (int((event->scenePos().x()-mousePressPoint.x())/gridX)*gridX)  - i->pos().x();
        int y = (int((event->scenePos().y()- mousePressPoint.y())/gridY)*gridY)  - i->pos().y();
        i->setPos(i->pos().x()+x,i->pos().y()+y);

    } else if(selectedItems().count() > 1 && mDrag){

        int x = (int((event->scenePos().x()-mousePressPoint.x())/gridX)*gridX)  - overlay->x();
        int y = (int((event->scenePos().y()- mousePressPoint.y())/gridY)*gridY)  - overlay->y();

        overlay->setPos(overlay->x()+x, overlay->y()+y);

        foreach(QGraphicsItem* i, selectedItems()) {
            i->setPos(i->pos().x()+x,i->pos().y()+y);
        }

    } else {
        QGraphicsScene::mouseMoveEvent(event);
    }
}


void MainScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{   qDebug () << selectedItems().count();
    if ( event->modifiers() == Qt::CTRL ){
       copy=true;
    }

    QGraphicsScene::mousePressEvent(event);
    if(selectedItems().count() > 1 && !mScale && !mRotate){
        mousePressPoint = event->scenePos() - overlay->pos();
        mDrag =true;
    }else if(selectedItems().count() == 1 && !mScale && !mRotate){
        mousePressPoint = event->scenePos() - selectedItems().front()->pos();
        mDrag =true;
    }
}


void MainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    mDrag =false;
    copy=false;
}

void MainScene::calcOverLayBounds()
{
    if(selectedItems().count() > 1) {
        int minX = 100000;
        int minY = 100000;
        int maxX = -100000;
        int maxY = -100000;

        foreach(QGraphicsItem* i, selectedItems()) {
            if(i->x() < minX){
                minX = i->pos().x();
            }

            if(i->pos().y() < minY){
                minY = i->pos().y();
            }

            if(i->pos().x()+i->boundingRect().width() > maxX){
                maxX = i->pos().x()+i->boundingRect().width();
            }

            if(i->pos().y()+i->boundingRect().height() > maxY){
                maxY = i->pos().y()+i->boundingRect().height();
            }
        }

        overlay->setRect(QRect(0,0, maxX-minX, maxY-minY));
        overlay->setPos(minX, minY);
        overlay->show();

    } else {
        overlay->hide();
    }
}



void MainScene::handleSelectionChanged() {
    calcOverLayBounds();
}
