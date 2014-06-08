#include "layoutwidget.h"
#include "ui_layoutwidget.h"

#include <QMainWindow>
#include <QSettings>

LayoutWidget::LayoutWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LayoutWidget)
{
    ui->setupUi(this);
    setWindowTitle ("Save current layout");

    // get the current stored layouts
    updateLayouts();
}

void LayoutWidget::accept()
{
    QSettings settings("layouts.ini", QSettings::IniFormat);

    QString name = ui->lineEdit->text().isEmpty()?
                "unnamed layout" : ui->lineEdit->text();
    QByteArray bytes = static_cast<QMainWindow*>(this->parent())->saveState();

    layouts.append(QPair<QString, QByteArray>(name, bytes));
    layoutsLength = layouts.count();

    settings.beginWriteArray("layouts");
    for (int i = 0; i < layoutsLength; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(
                    "layout_name",
                    layouts[i].first);
        settings.setValue(
                    "layout_array",
                    layouts[i].second);
    }
    settings.endArray();
    emit (accepted());
    this->close();
}

LayoutWidget::~LayoutWidget()
{
    delete ui;
}

void LayoutWidget::updateLayouts()
{
    QSettings settings("layouts.ini", QSettings::IniFormat);

    layoutsLength = settings.beginReadArray("layouts");
    qDebug() << "size =" << layoutsLength;
    for (int i = 0; i < layoutsLength; i++)
    {
        settings.setArrayIndex(i);
        layouts.append(
                    QPair<QString, QByteArray>(
                        settings.value("layout_name").toString(),
                        settings.value("layout_array").toByteArray())
                    );
        qDebug() << "index = " << i
                 << "cy1 = " << settings.value("layout_name","0").toString();

    }
    settings.endArray();
}

QList<QPair<QString, QByteArray> > LayoutWidget::getLayouts() {
    return layouts;
}
