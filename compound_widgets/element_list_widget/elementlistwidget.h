#ifndef ELEMENTLISTWIDGET_H
#define ELEMENTLISTWIDGET_H
#include <qtreeview.h>

#include <QWidget>

#include "mainscene.h"
#include "elementtreemodel.h"
#include "stable_pch.h"
#include "elementtreemodel.h"

namespace Ui {
class ElementListWidget;
}

class ElementListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ElementListWidget(QWidget *parent = 0);
    ~ElementListWidget();
    QTreeView *elementView;
    void setScene(MainScene *scene);
    void setModel(ElementTreeModel *model);
    void connectToScene();
    void disconnectScene();

    QTreeView *getElementView() const;
    void setElementView(QTreeView *value);

private slots:
    void handleOnItemsDeleted(QList<QGraphicsItem *> itemToRemove);
    void handleSceneSelectionChanged();
    void handleAddLayer();
    void handleRemoveLayer();
    void handleMoveLayerUp();
    void handleMoveLayerDown();
private:
    Ui::ElementListWidget *ui;
    MainScene *scene;
    ElementTreeModel *model;

};

#endif // ELEMENTLISTWIDGET_H
