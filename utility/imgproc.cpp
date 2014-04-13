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

std::vector<cv::Point> ImgProc::decimateVerticies(std::vector<cv::Point> src, int epsilon)
{
    std::vector<cv::Point> approxCurve;
    cv::approxPolyDP(src, approxCurve, epsilon, true);

    return approxCurve;
}

QList<QPair<unsigned,QPointF> > ImgProc::getMatchPoints(std::vector<std::vector<cv::Point> > contours,
                                                        const float &shape_thress,
                                                        const double &color_thress,
                                                        const QString &path)
{
    QList<QPair<unsigned,QPointF> > positions;

    QList<QList<unsigned> > matches = get_shapeMatches(contours,shape_thress);
    //QList<QList<unsigned> > matches = get_colorMatchesFromShapeMatches(shapeMatches, color_thress,path,contours);
    QList<unsigned> list;
    QPointF p;
    for(int i = 0; i< matches.size(); i++){
        list = matches.at(i);
        foreach(unsigned x, list){
            p = QPointF(cv::boundingRect(contours.at(x)).x,cv::boundingRect(contours.at(x)).y);
            positions.append(QPair< unsigned , QPointF>(i,p));
        }
    }
    return(positions);
}

QList<ItemTemplate*> ImgProc::splitImageAndRemoveDuplicates(std::vector<std::vector<cv::Point> > contours, const QString &path, float shapeTresh)
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    QList<QList<unsigned> > matches = get_shapeMatches(contours, shapeTresh);


    QList<unsigned> keepers;
    unsigned currentKeeper;
    int biggestSize;
    int tmpSize;

    for(int i=0; i < matches.size(); i++){
        QList<unsigned> list = matches.at(i);
        biggestSize = cv::minAreaRect(contours.at(list.at(0))).size.area();
        currentKeeper=list.at(0);
        if(list.size() > 1){
            for(int j = 1; j < list.size();j++){
                tmpSize = cv::minAreaRect(contours.at(list.at(j))).size.area();
                if( tmpSize > biggestSize){
                    biggestSize = tmpSize;
                    currentKeeper = list.at(j);
                }
            }
        }
        keepers.append(currentKeeper);
    }
    cv::Mat tmpImg;
    cv::Rect ROI;
    std::vector< std::vector<cv::Point> > currentContour;
    QList <ItemTemplate*> templates;
    ItemTemplate* currentTemplate;
    foreach(unsigned x, keepers){
        ROI = cv::boundingRect(contours.at(x));
        currentContour = std::vector< std::vector<cv::Point> > (1,contours.at(x));

        // CV_FILLED fills the connected components found
        cv::Mat crop;
        cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        cv::drawContours(mask, currentContour, -1, cv::Scalar(255), CV_FILLED);
        img.copyTo(crop,mask);
        tmpImg = crop(ROI);
        //GET THE NAME MAYBE MAKE A FOR LOOP RATHER THAN FOR EACH to get an int
        currentTemplate = new ItemTemplate("blabla",ItemTemplate::Single);
        currentTemplate->setImage(toQImage(tmpImg));
        currentTemplate->setContour(findContoursFromQImage(currentTemplate->image()));
        currentTemplate->setConvex(findConvexesFromQImage(currentTemplate->image()));
        currentTemplate->setPixmap (new QPixmap(
                                        QPixmap::fromImage(currentTemplate->image ())));

        currentTemplate->setPixmapItem(new QGraphicsPixmapItem(*currentTemplate->pixmap()));

        currentTemplate->calculateSceneRect();

        QBrush brush;
        brush.setTextureImage(QImage(":/images/checkerboard"));

        currentTemplate->scene ()->setBackgroundBrush(brush);
        currentTemplate->scene()->addItem( currentTemplate->getPixmapItem());
        currentTemplate->setIcon (QIcon(*currentTemplate->pixmap()));

        templates.append(currentTemplate);
    }
    return templates;
}

QList<ItemTemplate *> ImgProc::splitImage(std::vector<std::vector<cv::Point> > contours, const QString &path)
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    cv::Mat tmpImg;

    std::vector< std::vector<cv::Point> > currentContour;


    // CV_FILLED fills the connected components found
    cv::Rect ROI;
    QList <ItemTemplate*> templates;
    ItemTemplate* currentTemplate;

    for(unsigned i = 0; i< contours.size();i++){

        ROI = cv::boundingRect(contours.at(i));
        currentContour = std::vector< std::vector<cv::Point> > (1,contours.at(i));

        // CV_FILLED fills the connected components found
        cv::Mat crop;
        cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        cv::drawContours(mask, currentContour, -1, cv::Scalar(255), CV_FILLED);
        img.copyTo(crop,mask);
        tmpImg = crop(ROI);
        //GET THE NAME MAYBE MAKE A FOR LOOP RATHER THAN FOR EACH to get an int
        currentTemplate = new ItemTemplate("blabla", ItemTemplate::Single);
        currentTemplate->setImage(toQImage(tmpImg));
        currentTemplate->setContour(findContoursFromQImage(currentTemplate->image()));
        currentTemplate->setConvex(findConvexesFromQImage(currentTemplate->image()));
        currentTemplate->setPixmap (new QPixmap(
                                        QPixmap::fromImage(currentTemplate->image ())));

        currentTemplate->setPixmapItem(new QGraphicsPixmapItem(*currentTemplate->pixmap()));

        currentTemplate->calculateSceneRect();

        QBrush brush;
        brush.setTextureImage(QImage(":/images/checkerboard"));

        currentTemplate->scene ()->setBackgroundBrush(brush);
        currentTemplate->scene()->addItem( currentTemplate->getPixmapItem());
        currentTemplate->setIcon (QIcon(*currentTemplate->pixmap()));

        templates.append(currentTemplate);
    }
    return templates;
}


QList<QList<unsigned> > ImgProc::get_shapeMatches(std::vector<std::vector<cv::Point> > contours,
                                                  const float &shape_thress)
{
    unsigned max = contours.size();
    std::vector<unsigned> ignored;
    QList<QList<unsigned> > matched;

    bool ignore = false;
    for(unsigned i=0; i<max; i++)
    {
        ignore = false;
        foreach (unsigned x, ignored)
        {
            if((x == i)){
                ignore = true;
                break;
            }
        }

        if(!ignore)
        {
            QList<unsigned> list;
            list.append(i);

            for(unsigned j=i+1; j<max; j++)
            {
                foreach (unsigned x, ignored)
                {
                    if((x == j)){
                        ignore = true;
                        break;
                    }
                }

                if(!ignore) {
                    double match = cv::matchShapes(contours[i],
                                                   contours[j],
                                                   CV_CONTOURS_MATCH_I2, 0);

                    if(match < shape_thress)
                    {
                        list.append(j);

                        ignored.push_back(j);
                        ignored.push_back(i);
                    }
                } else ignore=false;
            }
            matched.append(list);
        }
    }
    return matched;
}

QList<QList<unsigned> > ImgProc::get_colorMatchesFromShapeMatches(QList<QList<unsigned> > shapeMatches,
                                                                  const double &color_tress,
                                                                  const QString &path,
                                                                  std::vector<std::vector<cv::Point> > contours)
{
    cv::Mat img = cv::imread(path.toStdString(), 1);
    cv::Mat tmpImg;
    cv::Rect ROI;
    QList<cv::MatND*> histograms;
    QList<QList<unsigned> > newMatchList;

    //Using 30 bins for hue and 32 for saturation
    int h_bins = 50; int s_bins = 60;
    int histSize[] = { h_bins, s_bins };
    // hue varies from 0 to 256, saturation from 0 to 180
    float h_ranges[] = { 0, 256 };
    float s_ranges[] = { 0, 180 };

    const float* ranges[] = { h_ranges, s_ranges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };

    foreach(QList<unsigned> matchList, shapeMatches){
        histograms.clear();
        std::vector<unsigned> ignored;
        bool ignore = false;

        //QList<QList<unsigned> > colorMatches;
        foreach(unsigned match, matchList){
            ROI = cv::boundingRect(contours.at(match));
            tmpImg = img(ROI);
            cvtColor( tmpImg, tmpImg, CV_BGR2HSV );
            cv::MatND* curHis = new cv::MatND();
            calcHist( &tmpImg, 1, channels, cv::Mat(), *curHis, 2, histSize, ranges, true, false );
            normalize( *curHis, *curHis, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
            histograms.append(curHis);
        }

        for (int j = 0; j < histograms.size(); j++){

            foreach (unsigned x, ignored)
            {
                if(( x == j)){
                    ignore = true;
                    break;
                }
            }
            if(!ignore){
                QList<unsigned> list;
                list.append(j);
                for(unsigned i = j+1; i < histograms.size(); i++){
                    foreach (unsigned x, ignored)
                    {
                        if(( x == i)){
                            ignore = true;
                            break;
                        }
                    }
                    if(!ignore){
                        if(cv::compareHist( *histograms.at(j), *histograms.at(i), CV_COMP_CHISQR ) < color_tress){
                            list.append(i);

                            ignored.push_back(j);
                            ignored.push_back(i);

                        }
                    } else {
                        ignore =false;
                    }
                    newMatchList.append(list);
                }
            } else {
                ignore =false;
            }
        }
    }
    return (newMatchList);
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

std::vector<std::vector<cv::Point> > ImgProc::findContoursFromQImage(QImage QImg)
{
    cv::Mat img = QImage2Mat(QImg);
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

std::vector<std::vector<cv::Point> > ImgProc::findConvexesFromQImage(QImage QImg)
{
    cv::Mat img = QImage2Mat(QImg);
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
    cv::cvtColor(img, temp, CV_BGRA2RGBA);

    QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGBA8888);
    QImage dest2(dest);
    dest2.detach(); // enforce deep copy
    return dest2;
}

cv::Mat ImgProc::QImage2Mat(QImage const& src)
{
    cv::Mat tmp(src.height(),src.width(),CV_8UC4,(uchar*)src.bits(),src.bytesPerLine());
    cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
    //cvtColor(tmp, result,CV_RGBA2BGRA);

    return tmp;
}


