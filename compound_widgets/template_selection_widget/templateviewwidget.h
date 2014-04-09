#ifndef TEMPLATEVIEWWIDGET_H
#define TEMPLATEVIEWWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QLayout>
#include <QIcon>
#include <QListWidget>

#include "thumbnailgridview.h"
#include "delegates/templatedelegate.h"

namespace Ui {
class TemplateViewWidget;
}

class TemplateViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TemplateViewWidget(QWidget *parent = 0);
    ~TemplateViewWidget();

private:
    Ui::TemplateViewWidget *ui;
    QStandardItemModel *model;
    QListWidget *listWidget;
    ThumbnailGridView *tiledListView;
    void populateModel(QStandardItemModel *model);
    QHBoxLayout *layout;
};

#endif // TEMPLATEVIEWWIDGET_H
