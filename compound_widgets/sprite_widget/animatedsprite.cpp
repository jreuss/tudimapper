#include "animatedsprite.h"

AnimatedSprite::AnimatedSprite(const QString& image, QGraphicsObject *parent) :
    QGraphicsObject(parent)
{
    mImage.load(image);
    mSeqStart = QPair<int,int>(0,0);
    mSeqEnd = QPair<int,int>(0,0);
    mSubImagePosition.setRect(0,0,mImage.width(),mImage.height());
    animate = false;
}

AnimatedSprite::AnimatedSprite(const AnimatedSprite &other) :  QGraphicsObject(0)
{
    mImage = QPixmap(other.getImage());
    mSubImagePosition = QRect(other.getSubImagePosition());
    mSpriteHeight = other.getSpriteHeight();
    mSpriteWidth = other.getSpriteWidth();
    mNumColumns = other.getNumColumns();
    mNumRows = other.getNumRows();
    mStartFrame = other.getStartFrame();
    mEndFrame = other.getEndFrame();
    mCurrentFrame = other.getCurrentFrame();
    mSeqStart = other.getSeqStart();
    mSeqEnd = other.getSeqEnd();
}

void AnimatedSprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // supress -wunused warnings
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(animate) {
        painter->drawPixmap(QRect(0, 0, mSpriteWidth,mSpriteHeight), mImage, mSubImagePosition );
        update();
    } else {
        //painter->drawPixmap(mImage.rect(), mImage);
    }
}

QRectF AnimatedSprite::boundingRect() const
{
    return QRectF(0,0, mSubImagePosition.width(), mSubImagePosition.height());
}

void AnimatedSprite::setAnimationProperties(int width, int height)
{
    mSpriteWidth = width;
    mSpriteHeight = height;
}

void AnimatedSprite::updateAnimation(AnimationsSequence *seq)
{
    mSeqStart = QPair<int,int>(seq->getStartRow()-1, seq->getStartColumn()-1);
    mSeqEnd = QPair<int,int>(seq->getEndRow()-1, seq->getEndColumn()-1);

    mEndFrame = mSeqEnd.first * mNumColumns + mSeqEnd.second;
    mStartFrame = mSeqStart.first * mNumColumns + mSeqStart.second;
    mCurrentFrame = 0;
}

void AnimatedSprite::setRowColumnCount(int rows, int columns)
{
    mNumColumns = columns;
    mNumRows = rows;
}

void AnimatedSprite::toggleAnimate(bool toggle)
{
    animate = toggle;
}

void AnimatedSprite::handleTimeStep()
{
    mCurrentFrame = (mCurrentFrame+1) % (mEndFrame-mStartFrame+1);

    int frame = mStartFrame+mCurrentFrame;
    int dx = (frame)%mNumColumns;
    int dy = (frame)/mNumColumns;

    prepareGeometryChange();
    mSubImagePosition = QRect(dx*mSpriteWidth,
                              dy*mSpriteHeight,
                              mSpriteWidth,mSpriteHeight);
    update();
}
QPair<int, int> AnimatedSprite::getSeqEnd() const
{
    return mSeqEnd;
}

void AnimatedSprite::setSeqEnd(const QPair<int, int> &value)
{
    mSeqEnd = value;
}

QPair<int, int> AnimatedSprite::getSeqStart() const
{
    return mSeqStart;
}

void AnimatedSprite::setSeqStart(const QPair<int, int> &value)
{
    mSeqStart = value;
}

int AnimatedSprite::getCurrentFrame() const
{
    return mCurrentFrame;
}

void AnimatedSprite::setCurrentFrame(int value)
{
    mCurrentFrame = value;
}

int AnimatedSprite::getEndFrame() const
{
    return mEndFrame;
}

void AnimatedSprite::setEndFrame(int value)
{
    mEndFrame = value;
}

int AnimatedSprite::getStartFrame() const
{
    return mStartFrame;
}

void AnimatedSprite::setStartFrame(int value)
{
    mStartFrame = value;
}

int AnimatedSprite::getNumRows() const
{
    return mNumRows;
}

void AnimatedSprite::setNumRows(int value)
{
    mNumRows = value;
}

int AnimatedSprite::getNumColumns() const
{
    return mNumColumns;
}

void AnimatedSprite::setNumColumns(int value)
{
    mNumColumns = value;
}

int AnimatedSprite::getSpriteHeight() const
{
    return mSpriteHeight;
}

void AnimatedSprite::setSpriteHeight(int value)
{
    mSpriteHeight = value;
}

int AnimatedSprite::getSpriteWidth() const
{
    return mSpriteWidth;
}

void AnimatedSprite::setSpriteWidth(int value)
{
    mSpriteWidth = value;
}

QRect AnimatedSprite::getSubImagePosition() const
{
    return mSubImagePosition;
}

void AnimatedSprite::setSubImagePosition(const QRect &value)
{
    mSubImagePosition = value;
}

QPixmap AnimatedSprite::getImage() const
{
    return mImage;
}

void AnimatedSprite::setImage(const QPixmap &value)
{
    mImage = value;
}



