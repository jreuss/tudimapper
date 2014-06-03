#ifndef ITEMTEMPLATE_H
#define ITEMTEMPLATE_H

#include <opencv2/core/core.hpp>

#include "compound_widgets/collider_widget/colliders/boxcollider.h"
#include "compound_widgets/collider_widget/colliders/collider.h"
#include "compound_widgets/collider_widget/colliderscene.h"
#include "items/abstracttreegraphicsitem.h"
#include "compound_widgets/sprite_widget/animation.h"
#include "abstracttreeitem.h"
#include "compound_widgets/split_widget/splitscene.h"

class ItemTemplate : public AbstractTreeItem
{
public:
    /* ======================================
     * region: type definitions
     * ====================================*/

    enum ImportType {
        None, Single, Group, SpriteSheet, Split, Folder
    };

    /* ======================================
     * region: c/d-tors
     * ====================================*/

    ItemTemplate(const QString &name=QString(), const ItemTemplate::ImportType
                   &type=None, AbstractTreeItem *parent=0);

    ~ItemTemplate();

    /* ======================================
     * region: child utils
     * ====================================*/



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

    QIcon icon() const;

    void setIcon(const QIcon &ico);

    QImage image() const;

    void setImage(const QImage &img);

    ColliderScene *scene() const;

    void calculateSceneRect();

    AbstractTreeGraphicsItem *getColliderRoot();

    void setPixmap(QPixmap *pixmap);

    QPixmap *pixmap() const;

    AbstractTreeGraphicsItem *getGetColliderRoot() const;
    void setGetColliderRoot(AbstractTreeGraphicsItem *value);

    Animation *getAnimation() const;
    void setAnimation(Animation *value);

    QGraphicsPixmapItem *getPixmapItem() const;
    void setPixmapItem(QGraphicsPixmapItem *value);

    QGraphicsScene *getSplitScene() const;
    void setSplitScene(QGraphicsScene *value);

    bool getIsSplit() const;
    void setIsSplit(bool value);

private:

    /* =======================================
     * region: private vars
     * =====================================*/
    QString mName, mPath;

    ColliderScene *mScene;

    QGraphicsScene *mSplitScene;

    bool mIsSplit;

    Animation *mAnimation;

    QImage mImg;
    QIcon mIco;
    QPixmap *mPixmap;
    QGraphicsPixmapItem *mPixmapItem;

    std::vector<std::vector<cv::Point> > mContour;
    std::vector<std::vector<cv::Point> > mConvex;
    std::vector<std::vector<int> > mMatchList;

    ImportType mType;
};

#endif // ITEMTEMPLATE_H
