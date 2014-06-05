#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H

#include "stable_pch.h"
#include <QDebug>

class TemplateDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    TemplateDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QPixmap folderIcon, groupOverlay, spriteOverlay;
};

#endif
