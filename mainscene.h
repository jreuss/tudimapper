#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QRubberBand>

#include "elementtreemodel.h"
#include "compound_widgets/template_selection_widget/thumbnailgridview.h"
#include "items/itemelement.h"

#include "items/abstractsceneitem.h"
#include "utility/zoomedgraphicview.h"

class ElementTreeModel;
class MainScene : public AbstractSceneItem
{
     Q_OBJECT
public:
    MainScene();

    ElementTreeModel *model;
    AbstractTreePixmapItem *getRoot() const;
    void setRoot(AbstractTreePixmapItem *value);
    void updatePos();

    QString getName() const;
    void setName(const QString &value);
    bool showColliders();
    void setShowColliders(bool set);
    bool rotate();
    void setRotate(bool set);
    bool scale();
    void setScale(bool set);
    void calcOverLayBounds();
signals:
    void onRequestTemplates(QPointF);
    void onItemDeleted(QList<QGraphicsItem*>);
    void onUpdatePos();
private slots:
    void handleSelectionChanged();

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
private:
    AbstractTreePixmapItem *mRoot;
    void keyPressEvent(QKeyEvent *event);
    QString mName;
    bool mShowColliders;
    bool mRotate;
    bool mScale;
    bool mDrag;
    int gridX, gridY;
    QPointF mousePressPoint;
    bool copy;

    QGraphicsRectItem *overlay;

    // QGraphicsScene interface
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

    // QGraphicsScene interface
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MAINSCENE_H
