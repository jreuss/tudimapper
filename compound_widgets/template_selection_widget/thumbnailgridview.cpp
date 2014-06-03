
#include "thumbnailgridview.h"
#include "items/itemtemplate.h"



namespace {
const int ExtraHeight = 3;
}

ThumbnailGridView::ThumbnailGridView(QWidget *parent)
    : QAbstractItemView(parent), mIdealWidth(0), mIdealHeight(0),
      mHashIsDirty(false)
{
    setFocusPolicy(Qt::WheelFocus);
    setFont(QApplication::font("QListView"));
    horizontalScrollBar()->setRange(0, 0);
    verticalScrollBar()->setRange(0, 0);

    // default size properties
    mItemHeight = 100;
    mItemWidth = 80;
    mPadding = 5;

    setDragDropMode(QAbstractItemView::DragDrop);
    setAcceptDrops(true);
    setDragEnabled(true);
}


void ThumbnailGridView::setModel(QAbstractItemModel *model)
{
    QAbstractItemView::setModel(model);
    mHashIsDirty = true;

}

void ThumbnailGridView::calculateRectsIfNecessary() const
{
    if (!mHashIsDirty)
        return;

    const int MaxWidth = viewport()->width();
    const int rowCount = model()->rowCount(displayIndex/*rootIndex()*/);

    int x = 5;
    int y = 5;

    // reset
    mColumnCount = 0;

    for (int row = 0; row < rowCount; ++row) {

        if (!(x == 5 || x + mItemWidth < MaxWidth)) {
            y += mItemHeight + mPadding;
            x = mPadding;
        } else if (x != mPadding) {
            x += mPadding;
        }

        mRectForRow[row] = QRectF(x, y, mItemWidth, mItemHeight);
        x += mItemWidth;

        // count columns on first row
        if(y == 5) {
            mColumnCount++;
        }
    }

    mIdealWidth = mItemWidth + mPadding;
    mIdealHeight = y + mItemHeight + mPadding;
    mHashIsDirty = false;
    viewport()->update();
}

QRect ThumbnailGridView::visualRect(const QModelIndex &index) const
{
    QRect rect;

    if (index.isValid()) {
        rect = viewportRectForRow(index.row()).toRect();
    }
    return rect;
}


QRectF ThumbnailGridView::viewportRectForRow(int row) const
{
    calculateRectsIfNecessary();

    QRectF rect = mRectForRow.value(row).toRect();

    if (!rect.isValid()) {
        return rect;
    }

    return QRectF(rect.x() - horizontalScrollBar()->value(),
                  rect.y() - verticalScrollBar()->value(),
                  rect.width(), rect.height());
}


void ThumbnailGridView::scrollTo(const QModelIndex &index,
                                 QAbstractItemView::ScrollHint)
{
    QRect viewRect = viewport()->rect();
    QRect itemRect = visualRect(index);

    if (itemRect.left() < viewRect.left())
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()
                                        + itemRect.left() - viewRect.left());
    else if (itemRect.right() > viewRect.right())
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()
                                        + qMin(itemRect.right() - viewRect.right(),
                                               itemRect.left() - viewRect.left()));
    if (itemRect.top() < viewRect.top())
        verticalScrollBar()->setValue(verticalScrollBar()->value() +
                                      itemRect.top() - viewRect.top());
    else if (itemRect.bottom() > viewRect.bottom())
        verticalScrollBar()->setValue(verticalScrollBar()->value() +
                                      qMin(itemRect.bottom() - viewRect.bottom(),
                                           itemRect.top() - viewRect.top()));
    viewport()->update();
}


QModelIndex ThumbnailGridView::indexAt(const QPoint &point_) const
{
    QPoint point(point_);

    point.rx() += horizontalScrollBar()->value();
    point.ry() += verticalScrollBar()->value();

    calculateRectsIfNecessary();

    QHashIterator<int, QRectF> i(mRectForRow);
    while (i.hasNext()) {
        i.next();
        if (i.value().contains(point))
            return model()->index(i.key(), 0, displayIndex/*rootIndex()*/);
    }
    return QModelIndex();
}


void ThumbnailGridView::dataChanged(const QModelIndex &topLeft,
                                    const QModelIndex &bottomRight
                                    )
{
    //mHashIsDirty = true;
    QAbstractItemView::dataChanged(topLeft, bottomRight);
    //viewport()->update();
}


void ThumbnailGridView::rowsInserted(const QModelIndex &parent, int start,
                                     int end)
{
    mHashIsDirty = true;
    QAbstractItemView::rowsInserted(parent, start, end);
   calculateRectsIfNecessary();
    //updateGeometries();
}


void ThumbnailGridView::rowsAboutToBeRemoved(const QModelIndex &parent,
                                             int start, int end)
{
    displayIndex = rootIndex();
    calculateRectsIfNecessary();
    mHashIsDirty = true;
    QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);

    //updateGeometries();

}

QModelIndex ThumbnailGridView::moveCursor(
        QAbstractItemView::CursorAction cursorAction,
        Qt::KeyboardModifiers)
{
    QModelIndex index = currentIndex();
    if (index.isValid()) {

        if ((cursorAction == MoveLeft && index.row() > 0) ||
                (cursorAction == MoveRight &&
                 index.row() + 1 < model()->rowCount())) {
            const int offset = (cursorAction == MoveLeft ? -1 : 1);
            index = model()->index(index.row() + offset,
                                   index.column(), index.parent());
        } else if ((cursorAction == MoveUp && index.row() > 0) ||
                 (cursorAction == MoveDown &&
                  index.row() + 1 < model()->rowCount())) {

            const int offset = (cursorAction == MoveUp ? -1 : 1);
            index = model()->index(index.row()+(offset*mColumnCount),
                                   index.column(), index.parent());
        }
    }
    return index;
}


int ThumbnailGridView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}


int ThumbnailGridView::verticalOffset() const
{
    return verticalScrollBar()->value();
}


void ThumbnailGridView::scrollContentsBy(int dx, int dy)
{
    scrollDirtyRegion(dx, dy);
    viewport()->scroll(dx, dy);
}


void ThumbnailGridView::setSelection(const QRect &rect,
                                     QFlags<QItemSelectionModel::SelectionFlag> flags)
{
    QRect rectangle = rect.translated(horizontalScrollBar()->value(),
                                      verticalScrollBar()->value()).normalized();
    calculateRectsIfNecessary();

    QHashIterator<int, QRectF> i(mRectForRow);
    int firstRow = model()->rowCount(displayIndex);
    int lastRow = -1;

    while (i.hasNext()) {
        i.next();
        if (i.value().intersects(rectangle)) {
            firstRow = firstRow < i.key() ? firstRow : i.key();
            lastRow = lastRow > i.key() ? lastRow : i.key();
        }
    }
    if (firstRow != model()->rowCount(displayIndex/*rootIndex()*/) && lastRow != -1) {
        QItemSelection selection(
                    model()->index(firstRow, 0, displayIndex/*rootIndex()*/),
                    model()->index(lastRow, 0, displayIndex/*rootIndex()*/));
        selectionModel()->select(selection, flags);
    }
    else {
        QModelIndex invalid;
        QItemSelection selection(invalid, invalid);
        selectionModel()->select(selection, flags);
    }
}

QRegion ThumbnailGridView::visualRegionForSelection(
        const QItemSelection &selection) const
{
    QRegion region;
    foreach (const QItemSelectionRange &range, selection) {
        for (int row = range.top(); row <= range.bottom(); ++row) {
            for (int column = range.left(); column < range.right();
                 ++column) {
                QModelIndex index = model()->index(row, column,
                                                   displayIndex/*rootIndex()*/);
                region += visualRect(index);
            }
        }
    }
    return region;
}


void ThumbnailGridView::paintEvent(QPaintEvent*)
{
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing|
                           QPainter::TextAntialiasing);

    for (int row = 0; row < model()->rowCount(displayIndex/*rootIndex()*/); ++row) {

        QModelIndex index = model()->index(row, 0, displayIndex/*rootIndex()*/);
        QRectF rect = viewportRectForRow(row);

        if (!rect.isValid() || rect.bottom() < 0 ||
                rect.y() > viewport()->height())
            continue;
        QStyleOptionViewItem option = viewOptions();
        option.rect = rect.toRect();

        if (selectionModel()->isSelected(index))
            option.state |= QStyle::State_Selected;
        if (currentIndex() == index)
            option.state |= QStyle::State_HasFocus;

        itemDelegate()->paint(&painter, option, index);
        paintOutline(&painter, rect);
    }
}


void ThumbnailGridView::paintOutline(QPainter *painter,
                                     const QRectF &rectangle)
{
    const QRectF rect = rectangle.adjusted(0, 0, -1, -1);
    painter->save();
    painter->setPen(QPen(palette().dark().color(), 0.5));
    painter->drawRect(rect);
    painter->setPen(QPen(Qt::black, 0.5));
    painter->restore();

}
int ThumbnailGridView::getPadding() const
{
    return mPadding;
}

void ThumbnailGridView::setPadding(int value)
{
    mPadding = value;
}

void ThumbnailGridView::forceUpdate()
{
    mHashIsDirty = true;
    calculateRectsIfNecessary();
    updateGeometries();
}

void ThumbnailGridView::setItemDimension(int itemWidth, int itemHeight, int itemPadding)
{
    mItemHeight = itemHeight;
    mItemWidth = itemWidth;
    mPadding = itemPadding;
}


void ThumbnailGridView::folderSelectionChanged(const QModelIndex &index)
{
    displayIndex = index;
    mHashIsDirty = true;
    calculateRectsIfNecessary();
    updateGeometries();
}

int ThumbnailGridView::getItemWidth() const
{
    return mItemWidth;
}

void ThumbnailGridView::setItemWidth(int value)
{
    mItemWidth = value;
}

int ThumbnailGridView::getItemHeight() const
{
    return mItemHeight;
}

void ThumbnailGridView::setItemHeight(int value)
{
    mItemHeight = value;
}

void ThumbnailGridView::resizeEvent(QResizeEvent*)
{
    mHashIsDirty = true;
    calculateRectsIfNecessary();
    updateGeometries();
}

void ThumbnailGridView::updateGeometries()
{
    QAbstractItemView::updateGeometries();
    QFontMetrics fm(font());
    const int RowHeight = mItemHeight + mPadding;
    horizontalScrollBar()->setSingleStep(fm.width("n"));
    horizontalScrollBar()->setPageStep(viewport()->width());
    horizontalScrollBar()->setRange(0,
                                    qMax(0, mIdealWidth - viewport()->width()));
    verticalScrollBar()->setSingleStep(RowHeight);
    verticalScrollBar()->setPageStep(viewport()->height());
    verticalScrollBar()->setRange(0,
                                  qMax(0, mIdealHeight - viewport()->height()));
}

void ThumbnailGridView::mousePressEvent(QMouseEvent *event)
{
    QAbstractItemView::mousePressEvent(event);
    // setCurrentIndex(indexAt(event->pos()));
}



