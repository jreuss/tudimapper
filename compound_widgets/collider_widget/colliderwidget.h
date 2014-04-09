#ifndef _H
#define _H

#include <QWidget>
#include <QRegExp>
#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QRect>
#include <QUuid>
#include <QMapIterator>
#include <QTimer>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QGraphicsItemGroup>

#include "colliders/collider.h"
#include "items/itemtemplate.h"
#include "colliders/meshcollider.h"
#include "colliders/boxcollider.h"
#include "colliders/circlecollider.h"
#include "colliders/meshnode.h"
#include "utility/imgproc.h"
#include "collidertreegraphicsmodel.h"

namespace Ui {
class ColliderWidget;
}

class ColliderWidget : public QWidget
{
    Q_OBJECT

public:
    enum ColliderType { BoxType = QGraphicsItem::UserType + 1,
                        MeshType = QGraphicsItem::UserType + 2,
                        CircleType = QGraphicsItem::UserType + 3,
                        NodeType = QGraphicsItem::UserType + 4};
    enum Toolbar{All, Transformations, Mesh };
    explicit ColliderWidget(QWidget *parent = 0);
    ~ColliderWidget();

    void setupConnections();
    void onLoadSelectedItem(ItemTemplate *item);
    void toggleAddColliderWidget ();
    void toggleScrollWidget();
    void toggleToolbarButtons(Toolbar btnGroup, bool enable);
    void toggleSceneSelection(bool enable);
    void toggleAdd();
    void toggleTreelist();
    void toogleUI(bool enable);
    void toggleSelectionTool(bool enable);
    void togglePointSelectionTool(bool enable);
    void deselectAllItems();

signals:
    void onToggleTreelistEnabled();

public slots:
    void handleCheckColliderOnTimerShot();

private slots:
    void handleImportItemFixtureDetailChanged(int);
    void handleAddBoxCollider();
    void handleAddCircleCollider ();
    void handleAddContourCollider ();
    void handleAddConvexCollider ();
    void handleAddCustomMeshCollider();
    void handleMeshDetailAccepted();
    void handleMeshDetailRejected();
    void handleSceneSelectionChanged();
    void handleSelectToolSelected(bool);
    void handleRotateToolSelected(bool);
    void handleScaleToolSelected(bool);
    void handleItemRemoved();
    void handleTreeviewSelectionChanged(QModelIndex);

private:
    bool onNonAcceptedMesh;
    QTimer mTimer;
    QUuid mSelectedColliderID;
    Ui::ColliderWidget *ui;
    ItemTemplate *mCurrentItem;
    QGraphicsRectItem *bg;
    ImgProc mImgProc;
    QBrush mBackgroundBrush;
    QGraphicsScene *mScene;
    ColliderTreeGraphicsModel *model;
    bool mContourFixture;
};


#endif // _H
