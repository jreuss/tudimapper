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

void SplitWidget::toogleUI(bool enable)
{
    ui->btn_split->setEnabled(enable);
    //FOR THE TEST
    //ui->btn_split->setHidden(true);
    ui->btn_splitAndAdd->setEnabled(enable);
    ui->gBox_rm_duplicates->setEnabled(enable);

}

void SplitWidget::setupConnections()
{
    connect(ui->slider_match_tresshold,SIGNAL(valueChanged(int)),
            this, SLOT(handleMatchTressholdChanged(int)));
    connect(ui->btn_split,SIGNAL(clicked()),this,SLOT(handleSplitAplied()));
    connect(ui->btn_splitAndAdd,SIGNAL(clicked()),this,SLOT(handleSplitAndAddToSceneAplied()));
    connect(this,SIGNAL(onToggleMainUIEnabled()),
            QWidget::window (),SLOT(handleToggleMainUIEnabled()));
}

void SplitWidget::onLoadSelectedItem(ItemTemplate *item)
{
    mCurrentItem = item;
    if(!mCurrentItem->getSplitScene()){
        mCurrentItem->setSplitScene(new SplitScene());
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

        mColorMatches = mImproc.get_colorMatches(0.25,mCurrentItem->path(),mCurrentItem->contour());
        handleMatchTressholdChanged(5);
        connect(mCurrentItem->getSplitScene(),SIGNAL(onDoubleClick(ItemTemplate*)),
                this, SLOT(handleDoubleClick(ItemTemplate*)));

    }
    if(!mCurrentItem->getIsSplit()){
        toogleUI(true);
        emit onToggleMainUIEnabled();
    } else {
        toogleUI(false);
    }
    ui->graphicsView->setScene(mCurrentItem->getSplitScene());

}

void SplitWidget::handleMatchTressholdChanged(int value)
{

    mShapeTreshVal = float( float(value) / 1000 )+0.001;
    QList<QPair<unsigned,QPointF> >  posList = mImproc.getMatchPoints(mCurrentItem->contour(),mShapeTreshVal,mColorMatches);
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
    mCurrentItem->setIsSplit(true);
    toogleUI(false);
    emit onToggleMainUIEnabled();
}

void SplitWidget::handleSplitAndAddToSceneAplied()
{
    emit onSplitAndAddToSceneAplied(mCurrentItem, ui->gBox_rm_duplicates->isChecked(), mShapeTreshVal);
    mCurrentItem->setIsSplit(true);
    toogleUI(false);
    emit onToggleMainUIEnabled();
}

void SplitWidget::handleDoubleClick(ItemTemplate *item)
{
    emit onDoubleClick(item);
}
