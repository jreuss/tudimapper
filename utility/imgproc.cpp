#include "imgproc.h"

const int ALPHA_THRESSHOLD = 1;

ImgProc::ImgProc() {}

void ImgProc::cvtAlphaToBinary(const cv::Mat &src, cv::Mat &out) const
{
    int nl= src.rows; // number of lines
    int nc= src.cols * src.channels();
    out = cv::Mat(src.rows, src.cols, CV_8UC1);

    if (src.isContinuous())
    {
        // then no padded pixels
        nc= nc*nl;
        nl= 1; // it is now a 1D array
    }

    // this loop is executed only once
    // in case of continuous images
    for (int j=0; j<nl; j++) {

        const uchar* data= src.ptr<uchar>(j);
        uchar * out_data = out.ptr<uchar>(j);

        for (int i=3; i<nc; i+=4) {
            *out_data++ = data[i] < ALPHA_THRESSHOLD ? 0 : 255;
        }

        data = NULL; delete data;
        out_data = NULL; delete out_data;
    }
}

QImage ImgProc::compare_test(std::vector<std::vector<cv::Point> > &contours,
                           const QString &path, const float& shape_thress)
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);

    int max = contours.size();
    cv::Scalar default_color = cv::Scalar(255,255,255, 255);
    std::vector<cv::Scalar> colors;

    colors.resize(max, default_color);
    cv::RNG rng(12345);

    for(int i=0; i<max; i++)
    {
       cv::Scalar col = cv::Scalar(rng.uniform(0, 255),
                                   rng.uniform(0, 255),
                                   rng.uniform(0, 255),
                                   255);

       for(int j=i+1; j<max; j++)
       {
           double match = cv::matchShapes(contours[i], contours[j], CV_CONTOURS_MATCH_I1, 0);
           if(match < shape_thress)
           {
               if(colors[j] == default_color)
               {
                    colors[j] = col;
                    colors[i] = col;
               }
           }
           else continue;
       }
    }

    for(unsigned i = 0; i< contours.size(); i++ )
    {
        cv::drawContours( img, contours,i,colors[i],5);
    }

    return toQImage(img);
}

QRectF ImgProc::getRect(std::vector<cv::Point>  &contour)
{
    cv::Rect rect = cv::boundingRect( cv::Mat(contour) );

    return QRectF(rect.x, rect.y, rect.width, rect.height);
}


//QList<TreeItem> ImgProc::split(const TreeItem &group, QTreeWidgetItem *parent)
//{
//    QList<TreeItem> list;
//    cv::Mat img = cv::imread(group.path.toStdString(), cv::IMREAD_UNCHANGED);
//    ElementTemplateManager templateManger;

//    qDebug() << group.matchList.size();

//    for(int i=0; i<group.matchList.size(); i++)
//    {
//        int index = group.matchList[i].front();

//        cv::Rect rect = cv::boundingRect(group.contours[index]);
//        cv::Mat mat = img(rect);

//        QImage qImg = toQImage(mat);

//        TreeItem treeItem;
//        treeItem.parent = parent;
//        treeItem.img = QImage(qImg);

//        std::vector<std::vector<cv::Point> > contour;
//        contour.push_back(group.contours[index]);

//        treeItem.contours = contour;
//        treeItem.name = "child item";
//        treeItem.itemType = "Single";

//        list.append(treeItem);
//    }

//    return list;
//}

std::vector<cv::Point> ImgProc::decimateVerticies(std::vector<cv::Point> src, int epsilon)
{
    std::vector<cv::Point> approxCurve;
    cv::approxPolyDP(src, approxCurve, epsilon, true);

    return approxCurve;
}

QList<QList<unsigned> > ImgProc::get_matches(std::vector<std::vector<cv::Point> > &contours,
                                             const float &shape_thress)
{
    unsigned max = contours.size();
    std::vector<unsigned> ignored;
    QList<QList<unsigned> > matched;

    bool ignore = false;
    for(unsigned i=0; i<max; i++)
    {
        foreach (unsigned x, ignored)
        {
            ignore = (x == i);
            break;
        }

        if(!ignore)
        {
            QList<unsigned> list;
            list.append(i);

            for(unsigned j=i+1; j<max; j++)
            {
                double match = cv::matchShapes(contours[i],
                                               contours[j],
                                               CV_CONTOURS_MATCH_I1, 0);
                if(match < shape_thress)
                {
                    list.append(j);
                    ignored.push_back(j);
                }
            }
            matched.append(list);
        }
    }

    return matched;
}

std::vector<std::vector<cv::Point> > ImgProc::findContours(const QString &path) const
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    cv::Mat img_out;

    std::vector<std::vector<cv::Point> > contours;

    if (! img.data )
    {
        qDebug() << "No image recieved!";
        return contours;
    }

    cvtAlphaToBinary(img, img_out);
    cv::findContours(img_out,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


    return contours;
}

std::vector<std::vector<cv::Point> > ImgProc::findConvexes(const QString &path) const
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    cv::Mat img_out;

    std::vector<std::vector<cv::Point> > contours;

    if (! img.data )
    {
        qDebug() << "No image recieved!";
        return contours;
    }

    cvtAlphaToBinary(img, img_out);
    cv::findContours(img_out,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    std::vector<std::vector<cv::Point> >hull( contours.size() );
      for( unsigned int i = 0; i < contours.size(); i++ )
         {  convexHull( cv::Mat(contours[i]), hull[i], false ); }

    return hull;
}

QImage ImgProc::toQImage(const cv::Mat &img)
{
    cv::Mat temp;
    cv::cvtColor(img, temp,CV_BGRA2RGBA);

    QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGBA8888);
    QImage dest2(dest);
    dest2.detach(); // enforce deep copy
    return dest2;
}




