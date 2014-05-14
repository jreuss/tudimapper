#include "colliderscene.h"
#include "colliderwidget.h"
#include "colliders/meshnode.h"
#include "colliders/meshcollider.h"

ColliderScene::ColliderScene(QObject *parent) :
    QGraphicsScene(parent)
{
    mRoot = new AbstractTreeGraphicsItem();
    mRoot->setZValue(2);
    this->addItem(mRoot);
    // set background brush
    QBrush brush;
    brush.setTextureImage(QImage(":/images/checkerboard"));
    QGraphicsRectItem *bg = new QGraphicsRectItem();
    bg->setBrush (brush);
    bg->setPen (Qt::NoPen);
    setBackgroundBrush(brush);
}

void ColliderScene::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Delete){
        foreach(QGraphicsItem *itm, this->selectedItems())
        {

            if(itm->type() == ColliderWidget::NodeType){
                MeshNode *node = static_cast<MeshNode *>(itm);
                MeshCollider *mc = static_cast<MeshCollider *>(node->parentItem ());
                int index = mc->mChildNodes.indexOf(node);
                mc->mChildNodes.takeAt(index);
                mc->updatePolygon();
                delete node;
            }  else if (itm->type() == ColliderWidget::MeshType){
                  MeshCollider *mc = static_cast<MeshCollider *>(itm);
                  if(mc->canBeDeleted()){
                     delete mc;
                  }
            } else if (itm->type() == ColliderWidget::BoxType || itm->type() == ColliderWidget::CircleType ) {
                delete itm;

            }
        }
        emit onItemDeleted();
    }
    QGraphicsScene::keyPressEvent(event);
}
AbstractTreeGraphicsItem *ColliderScene::getRoot()
{
    return mRoot;
}

void ColliderScene::setRoot(AbstractTreeGraphicsItem *value)
{
    mRoot = value;
}

