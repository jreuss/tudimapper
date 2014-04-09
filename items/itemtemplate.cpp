#include "itemtemplate.h"


ItemTemplate::ItemTemplate(const QString &name, const ItemTemplate::ImportType &type,
                               ItemTemplate *parent)
{
    if(parent) {
        parent->addChild (this);
    }

    mAnimation = NULL;
    mName = name;
    mParent = parent;
    mType = type;
    mScene = new ColliderScene();
}

ItemTemplate::~ItemTemplate()
{
    qDeleteAll(mChildren);
}

void ItemTemplate::addChild(ItemTemplate *child)
{
    child->setParent (this);
    mChildren << child;
}

void ItemTemplate::addChildren(const QList<ItemTemplate *> &children)
{
    foreach (ItemTemplate *child, children) {
        child->setParent (this);
        mChildren << child;
    }
}

ItemTemplate *ItemTemplate::childAt(int row) const
{
    return mChildren.value(row);
}

int ItemTemplate::rowOfChild(ItemTemplate *child) const
{
    return mChildren.indexOf (child);
}

bool ItemTemplate::hasChildren() const
{
    return mChildren.isEmpty ();
}

void ItemTemplate::removeChild(ItemTemplate *child)
{
    int index = mChildren.indexOf (child);

    if(index>0) {
        delete mChildren.takeAt (index);
    }
}

void ItemTemplate::deleteChildren()
{
    qDeleteAll(mChildren);
    mChildren.clear();
}

void ItemTemplate::removeAt(int position)
{
    delete mChildren.takeAt (position);
}

ItemTemplate *ItemTemplate::takeChild(int index)
{
    return mChildren.takeAt (index);
}

int ItemTemplate::childCount() const
{
    return mChildren.count ();
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

ItemTemplate *ItemTemplate::parent() const
{
    return mParent;
}

void ItemTemplate::setParent(ItemTemplate *parent)
{
    mParent = parent;
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

AbstractTreeGraphicsItem *ItemTemplate::getColliderRoot()
{
    return scene()->getRoot();
}

void ItemTemplate::setPixmap(QGraphicsPixmapItem *pixmap)
{
    mPixmap = pixmap;
}

QGraphicsPixmapItem *ItemTemplate::pixmap() const
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

