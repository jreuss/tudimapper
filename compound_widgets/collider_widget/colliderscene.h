#ifndef COLLIDERSCENE_H
#define COLLIDERSCENE_H


#include "stable_pch.h"
#include "items/abstracttreegraphicsitem.h"

class ColliderScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ColliderScene(QObject *parent = 0);

    AbstractTreeGraphicsItem *getRoot();
    void setRoot(AbstractTreeGraphicsItem *value);

signals:
    void onItemDeleted();

public slots:


    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);

private:
     AbstractTreeGraphicsItem *mRoot;
};

#endif // COLLIDERSCENE_H
