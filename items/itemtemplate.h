#ifndef ITEMTEMPLATE_H
#define ITEMTEMPLATE_H

#include <QList>
#include <QVector>
#include <QVariant>
#include <QDebug>
#include <QString>
#include <QIcon>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <opencv2/core/core.hpp>
#include <QUuid>

#include <compound_widgets/collider_widget/colliders/boxcollider.h>
#include <compound_widgets/collider_widget/colliders/collider.h>
#include <compound_widgets/collider_widget/colliderscene.h>
#include <items/abstracttreegraphicsitem.h>
#include <compound_widgets/sprite_widget/animation.h>

class ItemTemplate
{
public:
    /* ======================================
     * region: type definitions
     * ====================================*/

    enum ImportType {
        None, Single, Group, SpriteSheet, Decorator
    };

    /* ======================================
     * region: c/d-tors
     * ====================================*/

    ItemTemplate(const QString &name=QString(), const ItemTemplate::ImportType
                   &type=None, ItemTemplate *parent=0);

    ~ItemTemplate();

    /* ======================================
     * region: child utils
     * ====================================*/

    void addChild(ItemTemplate *child);

    void addChildren(const QList<ItemTemplate *> &children);

    ItemTemplate *childAt(int row) const;

    int rowOfChild(ItemTemplate *child) const;

    bool hasChildren() const;

    void removeChild(ItemTemplate *child);

    void deleteChildren();

    void removeAt(int position);

    ItemTemplate *takeChild(int index);

    int childCount() const;

    /* ======================================
     * region: getters & setters
     * ====================================*/

    void setName(const QString& name);

    QString name() const;

    void setItemType(ItemTemplate::ImportType type);

    ImportType importType() const;

    void setPath(const QString& path);

    QString path() const;

    std::vector<std::vector<cv::Point> > contour() const;

    void setContour(const std::vector<std::vector<cv::Point> >& contour);

    std::vector<std::vector<cv::Point> > convex() const;

    void setConvex(const std::vector<std::vector<cv::Point> > &convex);

    std::vector<std::vector<int> > matchList() const;

    void setMatchList(const std::vector<std::vector<int> >& matchList);


    ItemTemplate *parent() const;

    void setParent(ItemTemplate *parent);

    QIcon icon() const;

    void setIcon(const QIcon &ico);

    QImage image() const;

    void setImage(const QImage &img);

    ColliderScene *scene() const;

    AbstractTreeGraphicsItem *getColliderRoot();

    void setPixmap(QGraphicsPixmapItem *pixmap);

    QGraphicsPixmapItem *pixmap() const;

    AbstractTreeGraphicsItem *getGetColliderRoot() const;
    void setGetColliderRoot(AbstractTreeGraphicsItem *value);

    Animation *getAnimation() const;
    void setAnimation(Animation *value);

private:

    /* =======================================
     * region: private vars
     * =====================================*/
    QString mName, mPath;

    ColliderScene *mScene;
    Animation *mAnimation;

    QImage mImg;
    QIcon mIco;
    QGraphicsPixmapItem *mPixmap;

    std::vector<std::vector<cv::Point> > mContour;
    std::vector<std::vector<cv::Point> > mConvex;
    std::vector<std::vector<int> > mMatchList;

    ImportType mType;

    QList<ItemTemplate *> mChildren;
    ItemTemplate *mParent;
};

#endif // ITEMTEMPLATE_H
