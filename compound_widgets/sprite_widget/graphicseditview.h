#ifndef GRAPHICSEDITVIEW_H
#define GRAPHICSEDITVIEW_H

#include "stable_pch.h"
#include "compound_widgets/ruler_widget/graphicsviewrulerwidget.h"

class GraphicsEditView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsEditView(QWidget *parent = 0);

signals:

public slots:
private:
    GraphicsViewRulerWidget *mHorzRuler, *mVertRuler;

};

#endif // GRAPHICSEDITVIEW_H
