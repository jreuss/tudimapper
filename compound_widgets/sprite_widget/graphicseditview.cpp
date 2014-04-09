#include "graphicseditview.h"

GraphicsEditView::GraphicsEditView(QWidget *parent) :
    QGraphicsView(parent)
{
    setViewportMargins(20,20,0,0);
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    mHorzRuler = new GraphicsViewRulerWidget(GraphicsViewRulerWidget::Horizontal);
    mVertRuler = new GraphicsViewRulerWidget(GraphicsViewRulerWidget::Vertical);


    QWidget* fake = new QWidget();
    fake->setBackgroundRole(QPalette::Window);
    fake->setFixedSize(20,20);
    gridLayout->addWidget(fake,0,0);
    gridLayout->addWidget(mHorzRuler,0,1);
    gridLayout->addWidget(mVertRuler,1,0);
    gridLayout->addWidget(this->viewport(),1,1);

    this->setLayout(gridLayout);
}
