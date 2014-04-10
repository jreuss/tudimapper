#include "meshnode.h"
#include "meshcollider.h"

MeshNode::MeshNode(QGraphicsItem *parent)
{
    this->setParentItem (parent);
    mRect = new QRectF(0,0,5,5);
    mRect->translate (-mRect->center ());
    mFillColor = Qt::cyan;
    mOutlineColor = Qt::darkCyan;
    mSelectedColor = Qt::darkBlue;

    setFlags (ItemIsMovable | ItemIsSelectable |
              ItemSendsScenePositionChanges | ItemIsFocusable |
              ItemSendsGeometryChanges);
}

QRectF MeshNode::boundingRect() const
{
    return (*mRect);
}

QPainterPath MeshNode::shape() const
{
    QPainterPath path;
    path.addEllipse (*mRect);
    return path;
}

int MeshNode::type() const
{
    return QGraphicsItem::UserType+4;
}

void MeshNode::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                     QWidget *widget __attribute__((unused)))
{
    QPen pen(mOutlineColor);
    pen.setWidth (2);

    if(option->state & QStyle::State_Selected){
        pen.setColor (mSelectedColor);
        pen.setWidth (4);
    }

    painter->setPen (pen);
    painter->setBrush (mOutlineColor);
    painter->drawEllipse (*mRect);
}

QColor MeshNode::outlineColor() const
{
    return mOutlineColor;
}

void MeshNode::setOutlineColor(const QColor &color)
{
    mOutlineColor = color;
}

QColor MeshNode::fillColor() const
{
    return mFillColor;
}

void MeshNode::setFillColor(const QColor &color)
{
    mFillColor = color;
}

QVariant MeshNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
     if(change == ItemPositionHasChanged) {
         MeshCollider *mc = static_cast<MeshCollider *>(parentItem ());
         if(!mc->getScaleEnabled()){
            mc->updatePolygon ();
         }
     } else if (change == ItemSelectedHasChanged) {
         MeshCollider *mc = static_cast<MeshCollider *>(parentItem ());
         if(mc->indexOfChild (this) == 0) {
             mc->childCalledConsolidate ();
         }
     }

     return QGraphicsItem::itemChange(change, value);
}




