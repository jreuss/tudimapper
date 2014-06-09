#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QWidget>

namespace Ui {
class MainToolBar;
}

class MainToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit MainToolBar(QWidget *parent = 0);
    ~MainToolBar();

signals:
   void onColToggled(bool);
   void onTransToggled(bool);
   void onScaleToggled(bool);
   void onRotateToggled(bool);

public slots:
    void handleColToggled(bool);
    void handleTransToggled(bool);
    void handleScaleToggled(bool);
    void handleRotateToggled(bool);

private:
    Ui::MainToolBar *ui;

};

#endif // MAINTOOLBAR_H
