#include "itemtemplate.h"


ItemTemplate::ItemTemplate(const QString &name, const ItemTemplate::ImportType &type,
                               AbstractTreeItem *parent) : AbstractTreeItem(parent)
{    /*if(parent) {
            parent->addChild (this);
       }*/

    mAnimation = NULL;
    mName = name;
    mType = type;
    mScene = new ColliderScene();
    mSplitScene = NULL;
    mIsSplit = false;

}

ItemTemplate::~ItemTemplate()
{
    //    delete mScene;
    //    delete mSplitScene;
    //    delete mAnimation;
    //    delete mPixmap;
    //    delete mPixmapItem;
}

void ItemTemplate::setName(const QString &name)
{
    mName = name;
}

QString ItemTemplate::name() const
{
    return mName;
}

void ItemTemplate::setItemType(ItemTemplate::ImportType type)
{
    mType = type;
}

ItemTemplate::ImportType ItemTemplate::importType() const
{
    return mType;
}

void ItemTemplate::setPath(const QString &path)
{
    mPath = path;
}

QString ItemTemplate::path() const
{
    return mPath;
}

std::vector<std::vector<cv::Point> > ItemTemplate::contour() const
{
    return mContour;
}

void ItemTemplate::setContour(const std::vector<std::vector<cv::Point> > &contour)
{
    mContour = contour;
}

std::vector<std::vector<cv::Point> > ItemTemplate::convex() const
{
    return mConvex;
}

void ItemTemplate::setConvex(const std::vector<std::vector<cv::Point> > &convex)
{
    mConvex = convex;
}

std::vector<std::vector<int> > ItemTemplate::matchList() const
{
    return mMatchList;
}

void ItemTemplate::setMatchList(const std::vector<std::vector<int> > &matchList)
{
    mMatchList = matchList;
}

QIcon ItemTemplate::icon() const
{
    return mIco;
}

void ItemTemplate::setIcon(const QIcon &ico)
{
    mIco = ico;
}

QImage ItemTemplate::image() const
{
    return mImg;
}

void ItemTemplate::setImage(const QImage &img)
{
    mImg = img;
}

ColliderScene *ItemTemplate::scene() const
{
    return mScene;
}

void ItemTemplate::calculateSceneRect()
{
    mScene->setSceneRect(0,0,mPixmap->width()+20,mPixmap->height()+20);
    mScene->update();
}

AbstractTreeGraphicsItem *ItemTemplate::getColliderRoot()
{
    return scene()->getRoot();
}

void ItemTemplate::setPixmap(QPixmap *pixmap)
{
    mPixmap = pixmap;
}

QPixmap*ItemTemplate::pixmap() const
{
    return mPixmap;
}

Animation *ItemTemplate::getAnimation() const
{
    return mAnimation;
}

void ItemTemplate::setAnimation(Animation *value)
{
    mAnimation = value;
}
QGraphicsPixmapItem *ItemTemplate::getPixmapItem() const
{
    return mPixmapItem;
}

void ItemTemplate::setPixmapItem(QGraphicsPixmapItem *value)
{
    mPixmapItem = value;
}
QGraphicsScene *ItemTemplate::getSplitScene() const
{
    return mSplitScene;
}

void ItemTemplate::setSplitScene(QGraphicsScene *value)
{
    mSplitScene = value;
}
bool ItemTemplate::getIsSplit() const
{
    return mIsSplit;
}

void ItemTemplate::setIsSplit(bool value)
{
    mIsSplit = value;
}




