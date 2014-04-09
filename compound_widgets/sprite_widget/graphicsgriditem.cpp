#include "graphicsgriditem.h"

GraphicsGridItem::GraphicsGridItem(QRectF rect, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    mRect = rect;

    mHSpacing = mVSpacing = 0;
    mRows = mColumns = 0;

}

GraphicsGridItem::~GraphicsGridItem()
{

}

QRectF GraphicsGridItem::boundingRect() const
{
    return getGridRect();
}

void GraphicsGridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // suppress wunused
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen;
    pen.setColor(QColor(Qt::lightGray));

    painter->setPen(pen);

    painter->drawLines(mVLines);
    painter->drawLines(mHLines);
}

QRectF GraphicsGridItem::getGridRect() const
{
    return mRect;
}

void GraphicsGridItem::setGridRect(const QRectF &value)
{
    mRect = value;
}

void GraphicsGridItem::setGrid(int rows, int columns,
                               int verticalspacing, int horizontalspacing)
{
    mRows = rows;
    mColumns = columns;
    mVSpacing = verticalspacing;
    mHSpacing = horizontalspacing;
    updateGrid();
}


int GraphicsGridItem::getColumns() const
{
    return mColumns;
}

void GraphicsGridItem::setColumns(int value)
{
    mColumns = value;
    updateGrid();
}
int GraphicsGridItem::getRows() const
{
    return mRows;
}

void GraphicsGridItem::setRows(int value)
{
    mRows = value;
    updateGrid();
}
int GraphicsGridItem::getVSpacing() const
{
    return mVSpacing;
}

void GraphicsGridItem::setVSpacing(int value)
{
    mVSpacing = value;
    updateGrid();
}
int GraphicsGridItem::getHSpacing() const
{
    return mHSpacing;
}

void GraphicsGridItem::setHSpacing(int value)
{
    mHSpacing = value;
    updateGrid();
}

void GraphicsGridItem::updateGrid()
{
    prepareGeometryChange();

    // horizontal lines
    mHLines.clear();
    for(int i=0; i<=mRows; i++) {
        mHLines << QLine(QPoint(0, i*mHSpacing), QPoint(mRect.width(),i*mHSpacing));
    }

    // vertical lines
    mVLines.clear();
    for(int i=0; i<=mColumns; i++) {
        mVLines << QLine(QPoint(i*mVSpacing,0), QPoint(i*mVSpacing,mRect.height()));
    }
}





