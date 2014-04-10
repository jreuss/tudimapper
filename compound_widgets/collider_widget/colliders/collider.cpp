#include "collider.h"

Collider::Collider()
{
    mUniformScaleEnabled = false;
    mScaleFeedbackRectsSize = 6;
}

bool Collider::getScaleEnabled()
{
    return mScaleEnabled;
}

void Collider::setRotateEnabled(bool enable)
{
    mRotateEnabled = enable;
}
QString Collider::getName() const
{
    return mName;
}

void Collider::setName(const QString &value)
{
    mName = value;
}

bool Collider::getIsTrigger() const
{
    return mIsTrigger;
}

void Collider::setIsTrigger(bool value)
{
    mIsTrigger = value;
}

void Collider::drawScaleOverlay(QPainter *painter, QPen pen, QRectF outlineRect)
{
    painter->setBrush (Qt::NoBrush);
    painter->setOpacity (0.9);



    pen.setStyle (Qt::SolidLine);

    // draw rect bounds
    painter->drawRect (outlineRect);
    painter->setBrush (Qt::NoBrush);
    int cSize = mScaleFeedbackRectsSize;
    //Corners

    QVector<QRectF> rects;
    // The first four is the corners and the next four is the middle of the sides.
    rects << QRectF( outlineRect.topLeft ().x()-(cSize/2), outlineRect.topLeft ().y()-(cSize/2),cSize,cSize)
          << QRectF( outlineRect.bottomLeft ().x()-(cSize/2), outlineRect.bottomLeft ().y()-(cSize/2),cSize,cSize)
          << QRectF( outlineRect.topRight ().x()-(cSize/2), outlineRect.topRight ().y()-(cSize/2),cSize,cSize)
          << QRectF( outlineRect.bottomRight ().x()-(cSize/2), outlineRect.bottomRight ().y()-(cSize/2),cSize,cSize)

          << QRectF( outlineRect.bottomRight ().x()-(outlineRect.width()/2)-(cSize/2), outlineRect.bottomRight ().y()-(cSize/2),cSize,cSize)
          << QRectF( outlineRect.bottomRight ().x()-(outlineRect.width()/2)-(cSize/2), outlineRect.topRight().y()-(cSize/2),cSize,cSize)
          << QRectF( outlineRect.bottomLeft ().x()-(cSize/2), outlineRect.bottomRight ().y()-(outlineRect.height()/2)-(cSize/2),cSize,cSize)
          << QRectF( outlineRect.bottomRight().x()-(cSize/2), outlineRect.bottomRight ().y()-(outlineRect.height()/2)-(cSize/2),cSize,cSize);

    painter->setBrush (QColor(Qt::blue));
    pen.setStyle (Qt::SolidLine);
    painter->drawRects (rects);
}

QPointF Collider::getScaleOrigin(QVector2D vec, QRectF rect)
{
    mIsValidScaleOriginPoint = true;
    if(vec.distanceToPoint (QVector2D(rect.topLeft())) < 10) {
        mScaleXDirection = PosXAxis;
        mScaleYDirection = PosYAxis;
        if(mUniformScaleEnabled){
            mScaleType = Uniform;

        } else {
            mScaleType = BothXY;
        }
        mUniformXScaleVector = QVector2D(QPointF(rect.topLeft().x()-rect.bottomRight().x(),0));
        mUniformYScaleVector = QVector2D(QPointF(0,rect.topLeft().y()-rect.bottomRight().y()));
        setCursor (Qt::SizeFDiagCursor);
        return rect.bottomRight ();
    } else if(vec.distanceToPoint (QVector2D(rect.bottomLeft ())) < 10) {
        mScaleXDirection = PosXAxis;
        mScaleYDirection = NegYAxis;
        if(mUniformScaleEnabled){
            mScaleType = Uniform;
        } else {
            mScaleType = BothXY;
        }
        mUniformXScaleVector = QVector2D(QPointF(rect.bottomLeft().x()-rect.topRight().x(),rect.bottomLeft().y()-rect.topRight().y()));
        setCursor (Qt::SizeBDiagCursor);
        return rect.topRight ();
    } else if(vec.distanceToPoint (QVector2D(rect.topRight ())) < 10) {
        mScaleXDirection = NegXAxis;
        mScaleYDirection = PosYAxis;
        if(mUniformScaleEnabled){
            mScaleType = Uniform;
        } else {
            mScaleType = BothXY;
        }
        setCursor (Qt::SizeBDiagCursor);
        mUniformXScaleVector = QVector2D(QPointF(rect.topRight().x()-rect.bottomLeft().x(),rect.topRight().y()-rect.bottomLeft().y()));
        return rect.bottomLeft ();
    } else if(vec.distanceToPoint (QVector2D(rect.bottomRight ())) < 10) {
        mScaleXDirection = NegXAxis;
        mScaleYDirection = NegYAxis;
        if(mUniformScaleEnabled){
            mScaleType = Uniform;
        } else {
            mScaleType = BothXY;
        }
        mUniformXScaleVector = QVector2D(QPointF(rect.bottomRight().x()-rect.topLeft().x(),rect.bottomRight().y()-rect.topLeft().y()));
        setCursor (Qt::SizeFDiagCursor);
        return rect.topLeft ();
    } else if(vec.distanceToPoint (QVector2D(QPointF(rect.bottomRight ().x()-(rect.width()/2),rect.bottomRight ().y()))) < 10) {
        mScaleYDirection = NegYAxis;
        mScaleType = OnlyY;
        setCursor (Qt::SizeVerCursor);
        return (QPointF(rect.topRight ().x()-(rect.width()/2),rect.topRight ().y()));
    } else if(vec.distanceToPoint (QVector2D(QPointF(rect.topRight ().x()-(rect.width()/2),rect.topRight ().y()))) < 10) {
        mScaleYDirection = PosYAxis;
        mScaleType = OnlyY;
        setCursor (Qt::SizeVerCursor);
        return (QPointF(rect.bottomRight ().x()-(rect.width()/2),rect.bottomRight ().y()));
    } else if(vec.distanceToPoint (QVector2D(QPointF(rect.bottomRight ().x(),rect.bottomRight ().y()-(rect.height()/2)))) < 10) {
        mScaleXDirection = NegXAxis;
        mScaleType = OnlyX;
        setCursor (Qt::SizeHorCursor);
        return (QPointF(rect.bottomLeft ().x(),rect.bottomLeft ().y()-(rect.height()/2)));
    } else if(vec.distanceToPoint (QVector2D(QPointF(rect.bottomLeft ().x(),rect.bottomLeft ().y()-(rect.height()/2)))) < 10) {
        mScaleXDirection = PosXAxis;
        mScaleType = OnlyX;
        setCursor (Qt::SizeHorCursor);
        return (QPointF(rect.bottomRight ().x(),rect.bottomRight ().y()-(rect.height()/2))) ;
    }

    mIsValidScaleOriginPoint = false;
    setCursor (Qt::ArrowCursor);
    return QPointF();
}

void Collider::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Alt){
        mUniformScaleEnabled = true;
        if(mScaleType == BothXY){
            mScaleType = Uniform;
        }
    }
}

void Collider::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Alt){
        mUniformScaleEnabled = false;
        if(mScaleType == Uniform){
            mScaleType = BothXY;
        }
    }
}


//CHECK ALL BELOW AS THEY MAY NOT BE USED
//---------------------------------------
//Functions for vector calculations
QVector2D Collider::vectorProjection(QVector2D v1, QVector2D v2)
{
    return ((QVector2D::dotProduct(v1,v2)/(pow(v2.x(),2) + pow(v2.y(),2)))*v2);
}

float Collider::angleBetweenVectors(QVector2D v1, QVector2D v2)
{
    float dot = QVector2D::dotProduct(v1,v2);
    float det = v1.x ()*v2.y() - v1.y ()*v2.x();

    return (-qRadiansToDegrees(atan2(det,dot)));
}

//Rotates a single point around the center
QPointF Collider::rotatePoint(QPointF ptn, float rot)
{
    QMatrix mat = matrix().translate (boundingRect().center().x(),boundingRect().center().y())
            .rotate(rot)
            .translate(-boundingRect().center().x(), -boundingRect().center().y());
    return ptn * mat;
}

//Set up the scale Gui
void Collider::setUpScaleGui()
{
    mUniformScalePoint = boundingRect().center();

    QRectF tmpRect = QRectF(mUniformScalePoint-QPointF(5,5),mUniformScalePoint+QPointF(5,5));
    mScaleUniformPoly.clear();
    mScaleUniformPoly.append(rotatePoint(tmpRect.topLeft(), mRotation));
    mScaleUniformPoly.append(rotatePoint(tmpRect.topRight(), mRotation));
    mScaleUniformPoly.append(rotatePoint(tmpRect.bottomRight(), mRotation));
    mScaleUniformPoly.append(rotatePoint(tmpRect.bottomLeft(), mRotation));

    mXScalePoint = mUniformScalePoint;
    mXScalePoint.setX(mUniformScalePoint.x() + 50);
    tmpRect = QRectF(mXScalePoint-QPointF(5,5),mXScalePoint+QPointF(5,5));
    mScaleXPoly.clear();
    mScaleXPoly.append(rotatePoint(tmpRect.topLeft(), mRotation));
    mScaleXPoly.append(rotatePoint(tmpRect.topRight(), mRotation));
    mScaleXPoly.append(rotatePoint(tmpRect.bottomRight(), mRotation));
    mScaleXPoly.append(rotatePoint(tmpRect.bottomLeft(), mRotation));
    mXScalePoint = rotatePoint(mXScalePoint, mRotation);

    mYScalePoint = mUniformScalePoint;
    mYScalePoint.setY(mUniformScalePoint.y() - 50);
    tmpRect = QRectF(mYScalePoint-QPointF(5,5),mYScalePoint+QPointF(5,5));
    mScaleYPoly.clear();
    mScaleYPoly.append(rotatePoint(tmpRect.topLeft(), mRotation));
    mScaleYPoly.append(rotatePoint(tmpRect.topRight(), mRotation));
    mScaleYPoly.append(rotatePoint(tmpRect.bottomRight(), mRotation));
    mScaleYPoly.append(rotatePoint(tmpRect.bottomLeft(), mRotation));
    mYScalePoint = rotatePoint(mYScalePoint, mRotation);

    mUniformScalePoint = rotatePoint(mUniformScalePoint, mRotation);

    mScaleXAxis = QVector2D(mXScalePoint - mUniformScalePoint);
    mScaleYAxis = QVector2D(mYScalePoint - mUniformScalePoint);

}






