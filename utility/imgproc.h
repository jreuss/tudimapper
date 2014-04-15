#ifndef IMGPROC_H
#define IMGPROC_H

#include <QString>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QRect>
#include <QImage>
#include <QDebug>
#include <QPair>
#include <QList>
#include <QPointF>
#include <QGraphicsScene>

#include "items/itemtemplate.h"

class ImgProc
{
public:
    ImgProc();

    std::vector<std::vector<cv::Point> >  findContours(const QString &path) const;
    std::vector<std::vector<cv::Point> > findConvexes(const QString &path) const;
    std::vector<std::vector<cv::Point> >  findContoursFromQImage(QImage QImg);
    std::vector<std::vector<cv::Point> > findConvexesFromQImage(QImage QImg);

    QImage toQImage(const cv::Mat &img);
    cv::Mat QImage2Mat(QImage const& src);

    QImage compare_test(std::vector<std::vector<cv::Point> > &contours,
                        const QString &path, const float& shape_thress);

    QRectF getRect(std::vector<cv::Point> &contour);

   void getCircle(std::vector<cv::Point> &contour,  QPointF *center,float *radius);

    std::vector<cv::Point> decimateVerticies(std::vector<cv::Point> src, int epsilon);

    QList<QPair<unsigned,QPointF> > getMatchPoints(std::vector<std::vector<cv::Point> > contours,
                                                   const float &shape_thress,
                                                   QList<QList<unsigned> > colorMatches);

    QList<ItemTemplate*> splitImageAndRemoveDuplicates(
            std::vector<std::vector<cv::Point> > contours,
            const QString &path,
            float shapeTress,
            QList<QList<unsigned> > colorMatches);

    QList<ItemTemplate*> splitImage(
            std::vector<std::vector<cv::Point> > contours,
            const QString &path);

    QList<ItemTemplate *> getSplitTemplates(QList<unsigned> indexes,
                                            cv::Mat img,
                                            std::vector<std::vector<cv::Point> > contours);

    QPair<QList<ItemTemplate *>, QGraphicsScene*>  splitAndAddToScene(
            std::vector<std::vector<cv::Point> > contours,
            const QString &path,
            QGraphicsScene *baseScene);

    QPair<QList<ItemTemplate *>, QGraphicsScene*>  getSplitAndAddToSceneTemplates(QList<unsigned> indexes,
                                            cv::Mat img,
                                            std::vector<std::vector<cv::Point> > contours,
                                            QGraphicsScene *baseScene);


    QList<QList<unsigned> > get_shapeMatches(
            std::vector<std::vector<cv::Point> > contours,
            const float& shape_thress);

    QList<QList<unsigned> > get_shapeMatchesFromColorMatches(std::vector<std::vector<cv::Point> > contours,
                                                      const float &shape_thress, QList<QList<unsigned> > colorMatches);


    QList<QList<unsigned> > get_colorMatches(
            const double &color_tress,
            const QString &path,
            std::vector<std::vector<cv::Point> > contours);

private:
    void cvtAlphaToBinary(const cv::Mat &src, cv::Mat &out) const;
};

#endif // IMGPROC_H
