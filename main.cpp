#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QBitmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QPixmap pixmap(":/images/splash");
    QSplashScreen splash(pixmap);
    splash.show();

    a.processEvents();

    QTimer::singleShot(4000, &splash ,SLOT(close()));
    QTimer::singleShot(2000, &w ,SLOT(showMaximized()));

    return a.exec();
}
