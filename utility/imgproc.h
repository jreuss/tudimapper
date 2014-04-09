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

class ImgProc
{
public:
    ImgProc();

    std::vector<std::vector<cv::Point> >  findContours(const QString &path) const;
    std::vector<std::vector<cv::Point> > findConvexes(const QString &path) const;

    QImage toQImage(const cv::Mat &img);

    QImage compare_test(std::vector<std::vector<cv::Point> > &contours,
                        const QString &path, const float& shape_thress);

    QRectF getRect(std::vector<cv::Point> &contour);

   void getCircle(std::vector<cv::Point> &contour,  QPointF *center,float *radius);

    std::vector<cv::Point> decimateVerticies(std::vector<cv::Point> src, int epsilon);

    QList<QList<unsigned> > get_matches(
            std::vector<std::vector<cv::Point> > &contours,
            const float& shape_thress);
private:
    void cvtAlphaToBinary(const cv::Mat &src, cv::Mat &out) const;
};

#endif // IMGPROC_H
