#ifndef THUMBNAILGRIDVIEW_H
#define THUMBNAILGRIDVIEW_H


#include "stable_pch.h"


class ThumbnailGridView : public QAbstractItemView
{
    Q_OBJECT

public:
    explicit ThumbnailGridView(QWidget *parent=0);
    ~ThumbnailGridView() {}
    void setModel(QAbstractItemModel *model);
    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index,
                  QAbstractItemView::ScrollHint);
    QModelIndex indexAt(const QPoint &point_) const;

    int getItemHeight() const;
    void setItemHeight(int value);

    int getItemWidth() const;
    void setItemWidth(int value);

    int getPadding() const;
    void setPadding(int value);

    void forceUpdate();

    void setItemDimension(int mItemWidth, int mItemHeight, int itemPadding);

protected slots:
    void folderSelectionChanged(const QModelIndex &index);

    void dataChanged(const QModelIndex &topLeft,
                     const QModelIndex &bottomRight
                     );
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start,
                              int end);
    void updateGeometries();

protected:
    QModelIndex moveCursor(
            QAbstractItemView::CursorAction cursorAction,
            Qt::KeyboardModifiers modifiers);
    bool isIndexHidden(const QModelIndex&) const { return false; }
    int horizontalOffset() const;
    int verticalOffset() const;
    void scrollContentsBy(int dx, int dy);
    void setSelection(const QRect &rect,
            QFlags<QItemSelectionModel::SelectionFlag> flags);
    QRegion visualRegionForSelection(
            const QItemSelection &selection) const;

    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent *event);

private:
    void calculateRectsIfNecessary() const;
    QRectF viewportRectForRow(int row) const;
    void paintOutline(QPainter *painter, const QRectF &rectangle);

    int mItemHeight, mItemWidth, mPadding;

    mutable int  mColumnCount;
    mutable int mIdealWidth;
    mutable int mIdealHeight;
    mutable QHash<int, QRectF> mRectForRow;
    mutable bool mHashIsDirty;

    QModelIndex displayIndex;
};


#endif // THUMBNAILGRIDVIEW_H
