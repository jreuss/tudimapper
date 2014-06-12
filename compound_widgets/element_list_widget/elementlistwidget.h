#ifndef ELEMENTLISTWIDGET_H
#define ELEMENTLISTWIDGET_H

#include <QWidget>

namespace Ui {
class ElementListWidget;
}

class ElementListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ElementListWidget(QWidget *parent = 0);
    ~ElementListWidget();

private:
    Ui::ElementListWidget *ui;
};

#endif // ELEMENTLISTWIDGET_H
