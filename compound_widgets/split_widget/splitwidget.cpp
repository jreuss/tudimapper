#include "splitwidget.h"
#include "ui_splitwidget.h"

SplitWidget::SplitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplitWidget)
{
    mCurrentItem = NULL;
    ui->setupUi(this);

    ui->graphicsView->setBackgroundRole(QPalette::Dark);
    ui->graphicsView->setRenderHints (QPainter::Antialiasing |
                                      QPainter::HighQualityAntialiasing);
}

SplitWidget::~SplitWidget()
{
    delete ui;
}

void SplitWidget::setupConnections()
{
    connect(ui->slider_match_tresshold,SIGNAL(valueChanged(int)),
            this, SLOT(handleMatchTressholdChanged(int)));
    connect(ui->btn_split,SIGNAL(clicked()),this,SLOT(handleSplitAplied()));
}

void SplitWidget::onLoadSelectedItem(ItemTemplate *item)
{
    mCurrentItem = item;
    if(!mCurrentItem->getSplitScene()){
        mCurrentItem->setSplitScene(new QGraphicsScene());
        QBrush brush;
        brush.setTextureImage(QImage(":/images/checkerboard"));
        mCurrentItem->getSplitScene()->setBackgroundBrush(brush);
        mCurrentItem->getSplitScene()->setSceneRect(0,0,mCurrentItem->pixmap()->width(), mCurrentItem->pixmap()->height());
        mCurrentItem->getSplitScene()->addItem(new QGraphicsPixmapItem(*mCurrentItem->pixmap()));
        numFont.setPixelSize(40);
        for(unsigned i=0; i< mCurrentItem->contour().size(); i++){
            numberList.append(new QGraphicsTextItem());
            mCurrentItem->getSplitScene()->addItem(numberList.at(i));
              numberList.at(i)->setDefaultTextColor(Qt::red);
            numberList.at(i)->setFont(numFont);
        }


        handleMatchTressholdChanged(5);
    }

    ui->graphicsView->setScene(mCurrentItem->getSplitScene());
}

void SplitWidget::handleMatchTressholdChanged(int value)
{

    mShapeTreshVal = float( float(value) / 100 )+0.01;
    QList<QPair<unsigned,QPointF> >  posList = mImproc.getMatchPoints(mCurrentItem->contour(),mShapeTreshVal, 0.8,mCurrentItem->path());
    QPair<unsigned,QPointF> curPos;
    for(unsigned i=0; i < mCurrentItem->contour().size(); i++) {
        curPos = posList.at(i);
        unsigned num = curPos.first;
        numberList.at(i)->setPlainText(QString("%1").arg(num));
        numberList.at(i)->setPos(curPos.second);
    }

}

void SplitWidget::handleSplitAplied()
{
    emit onSplitAplied(mCurrentItem, ui->gBox_rm_duplicates->isChecked(), mShapeTreshVal);
}
