#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "stable_pch.h"
#include "animationssequence.h"

class AnimatedSprite : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit AnimatedSprite(const QString& image, QGraphicsObject *parent = 0);
    AnimatedSprite(const AnimatedSprite& other);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    void setAnimationProperties(int width, int height);
    void updateAnimation(AnimationsSequence *seq);
    void setRowColumnCount(int rows, int columns);
    void toggleAnimate(bool);

    QPixmap getImage() const;
    void setImage(const QPixmap &value);

    QRect getSubImagePosition() const;
    void setSubImagePosition(const QRect &value);

    int getSpriteWidth() const;
    void setSpriteWidth(int value);

    int getSpriteHeight() const;
    void setSpriteHeight(int value);

    int getNumColumns() const;
    void setNumColumns(int value);

    int getNumRows() const;
    void setNumRows(int value);

    int getStartFrame() const;
    void setStartFrame(int value);

    int getEndFrame() const;
    void setEndFrame(int value);

    int getCurrentFrame() const;
    void setCurrentFrame(int value);

    QPair<int, int> getSeqStart() const;
    void setSeqStart(const QPair<int, int> &value);

    QPair<int, int> getSeqEnd() const;
    void setSeqEnd(const QPair<int, int> &value);

signals:

public slots:
    void handleTimeStep();

private:
    bool animate;

    QPixmap mImage;
    QRect mSubImagePosition;
    int mSpriteWidth, mSpriteHeight, mNumColumns, mNumRows,
    mStartFrame, mEndFrame, mCurrentFrame;

    QPair<int,int> mSeqStart, mSeqEnd;
};

#endif // ANIMATEDSPRITE_H
