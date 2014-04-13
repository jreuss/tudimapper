#ifndef SPLITWIDGET_H
#define SPLITWIDGET_H

#include <QWidget>

namespace Ui {
class SplitWidget;
}

class SplitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SplitWidget(QWidget *parent = 0);
    ~SplitWidget();

private:
    Ui::SplitWidget *ui;
};

#endif // SPLITWIDGET_H
