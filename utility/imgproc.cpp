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
                                                        QList<QList<unsigned> > colorMatches)
{
    QList<QPair<unsigned,QPointF> > positions;

    QList<QList<unsigned> > matches = get_shapeMatchesFromColorMatches(contours,shape_thress,colorMatches);
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

QList<ItemTemplate*> ImgProc::splitImageAndRemoveDuplicates(std::vector<std::vector<cv::Point> > contours, const QString &path, float shapeTresh, QList<QList<unsigned> > colorMatches)
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    QList<QList<unsigned> > matches = get_shapeMatchesFromColorMatches(contours,
                                                                       shapeTresh,
                                                                       colorMatches);


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
    return (getSplitTemplates(keepers,img,contours));
}

QList<ItemTemplate *> ImgProc::splitImage(std::vector<std::vector<cv::Point> > contours, const QString &path)
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    QList<unsigned> keepers;
    for(unsigned i = 0; i < contours.size();i++)
    {
        keepers.append(i);
    }



    return (getSplitTemplates(keepers,img,contours));

}

QList<ItemTemplate *> ImgProc::getSplitTemplates(QList<unsigned> indexes, cv::Mat img, std::vector<std::vector<cv::Point> > contours)
{
    cv::Rect ROI;
    QList <ItemTemplate*> templates;
    ItemTemplate* currentTemplate;
    cv::Mat tmpImg;
    std::vector< std::vector<cv::Point> > currentContour;
    for(int i = 0; i< indexes.size();i++){

        ROI = cv::boundingRect(contours.at(indexes.at(i)));
        currentContour = std::vector< std::vector<cv::Point> > (1,contours.at(indexes.at(i)));

        cv::Mat crop;
        cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        cv::drawContours(mask, currentContour, -1, cv::Scalar(255), CV_FILLED);
        img.copyTo(crop,mask);
        tmpImg = crop(ROI);

        QString name;
        //GET THE NAME FROM TEMPLATE
        QTextStream(&name) << "NewTemplate" << i;
        currentTemplate = new ItemTemplate(name, ItemTemplate::Single);
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


QList<ItemTemplate *>  ImgProc::splitAndAddToScene(
        std::vector<std::vector<cv::Point> > contours,
        const QString &path, QGraphicsScene *baseScene)
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    QList<unsigned> keepers;
    for(unsigned i = 0; i < contours.size();i++)
    {
        keepers.append(i);
    }



    return (getSplitAndAddToSceneTemplates(keepers,img,contours, baseScene));

}

QList<ItemTemplate *> ImgProc::splitImageAndRemoveDuplicatesAddToScene(
        std::vector<std::vector<cv::Point> > contours,
        const QString &path,
        float shapeTresh, QList<QList<unsigned> > colorMatches,
        QGraphicsScene *baseScene)
{
    cv::Mat img = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    QList<QList<unsigned> > matches = get_shapeMatchesFromColorMatches(contours,
                                                                       shapeTresh,
                                                                       colorMatches);


    QList<QPair<unsigned, QList<unsigned> > > biggestAndMatches;
    unsigned biggest;
    int biggestSize;
    int tmpSize;

    for(int i=0; i < matches.size(); i++){
        QList<unsigned> list = matches.at(i);
        biggestSize = cv::minAreaRect(contours.at(list.at(0))).size.area();
        biggest=list.at(0);
        if(list.size() > 1){
            for(int j = 1; j < list.size();j++){
                tmpSize = cv::minAreaRect(contours.at(list.at(j))).size.area();
                if( tmpSize > biggestSize){
                    biggestSize = tmpSize;
                    biggest = list.at(j);
                }
            }
        }
        biggestAndMatches.append(QPair<unsigned, QList<unsigned> >(biggest,list));
    }
    return (getSplitAndRemoveAddToSceneTemplates(biggestAndMatches,img,contours, baseScene));
}



QList<ItemTemplate *>  ImgProc::getSplitAndAddToSceneTemplates(QList<unsigned> indexes,
                                                               cv::Mat img,
                                                               std::vector<std::vector<cv::Point> > contours,
                                                               QGraphicsScene *baseScene)
{
    cv::Rect ROI;
    QList <ItemTemplate*> templates;
    ItemTemplate* currentTemplate;
    cv::Mat tmpImg;
    std::vector< std::vector<cv::Point> > currentContour;
    baseScene->clear();

    for(int i = 0; i< indexes.size();i++){

        ROI = cv::boundingRect(contours.at(indexes.at(i)));
        currentContour = std::vector< std::vector<cv::Point> > (1,contours.at(indexes.at(i)));

        cv::Mat crop;
        cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        cv::drawContours(mask, currentContour, -1, cv::Scalar(255), CV_FILLED);
        img.copyTo(crop,mask);
        tmpImg = crop(ROI);
        QString name;
        //GET THE NAME FROM TEMPLATE
        QTextStream(&name) << "NewTemplate" << i;
        currentTemplate = new ItemTemplate(name, ItemTemplate::Single);
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

        ItemElement* imageForParentScene = new ItemElement(currentTemplate);

        baseScene->addItem(imageForParentScene);
        imageForParentScene->setFlag(QGraphicsItem::ItemIsSelectable);
        imageForParentScene->setFlag(QGraphicsItem::ItemIsMovable);
        imageForParentScene->setPos(ROI.x,ROI.y);
    }
    return templates;
}

QList<ItemTemplate *> ImgProc::getSplitAndRemoveAddToSceneTemplates(QList<QPair<unsigned, QList<unsigned> > > biggestAndMatches, cv::Mat img, std::vector<std::vector<cv::Point> > contours, QGraphicsScene *baseScene)
{
    cv::Rect ROI;
    QList <ItemTemplate*> templates;
    ItemTemplate* currentTemplate;
    cv::Mat tmpImg;
    std::vector< std::vector<cv::Point> > currentContour;
    unsigned currentBiggest;
    QList <unsigned> currentMatches;
    //float rotation;
    //float scale;

    baseScene->clear();

    for(int i = 0; i< biggestAndMatches.size();i++){
        currentBiggest = biggestAndMatches.at(i).first;
        currentMatches = biggestAndMatches.at(i).second;
        float biggestArea = float(cv::minAreaRect(contours.at(currentBiggest)).size.area());;


        ROI = cv::boundingRect(contours.at(currentBiggest));
        currentContour = std::vector< std::vector<cv::Point> > (1,contours.at(currentBiggest));

        cv::Mat crop;
        cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        cv::drawContours(mask, currentContour, -1, cv::Scalar(255), CV_FILLED);
        img.copyTo(crop,mask);
        tmpImg = crop(ROI);

        QString name;
        //GET THE NAME FROM TEMPLATE
        QTextStream(&name) << "NewTemplate" << i;
        currentTemplate = new ItemTemplate(name, ItemTemplate::Single);
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


//        cv::Rect matchROI;
//        cv::Mat matchRoiTmp;
        for(int j = 0; j < currentMatches.size();j++){

            ItemElement* imageForParentScene = new ItemElement(currentTemplate);
            std::vector<cv::Point> currentMatchContour = contours.at(currentMatches.at(j));

            //cv::RotatedRect rect = cv::minAreaRect(currentMatchContour);
           // cv::Size rect_size = rect.size;
            //NEED TO IMPLEMENT ROTATION
//            float angle = rect.angle;
//            if (rotation < -45.) {
//                rotation += 90.0;
//                //int tmpH = rect_size.height;
//                rect_size = cv::Size(rect_size.height,rect_size.width);
//                //swap(rect_size.width, rect_size.height);
//            };
//            cv::Mat M = cv::getRotationMatrix2D(rect.center, angle, 1);
//            cv::warpAffine(img,matchRoiTmp, M, img.size(), cv::INTER_CUBIC);
//            cv::Point pt(rect.center.x-(rect_size.width/2),rect.center.y-(rect_size.height/2));
//            matchROI = cv::Rect(pt, rect_size);
//            matchRoiTmp = matchRoiTmp(matchROI);


            float scale =sqrt(float(cv::minAreaRect(currentMatchContour).size.area())/biggestArea);
            scale = ceilf(scale * 100)/100;


            cv::Rect pos = cv::boundingRect(currentMatchContour);
            baseScene->addItem(imageForParentScene);
            imageForParentScene->setFlag(QGraphicsItem::ItemIsSelectable);
            imageForParentScene->setFlag(QGraphicsItem::ItemIsMovable);
            imageForParentScene->setPos(pos.x,pos.y);
            imageForParentScene->setTransformOriginPoint(imageForParentScene->boundingRect().x(),
                                                         imageForParentScene->boundingRect().y());
            imageForParentScene->setScale(scale);
            //imageForParentScene->setRotation(qreal(rotation));
        }
    }
    return QList<ItemTemplate *>(templates);
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

QList<QList<unsigned> > ImgProc::get_shapeMatchesFromColorMatches(std::vector<std::vector<cv::Point> > contours,
                                                                  const float &shape_thress, QList<QList<unsigned> > colorMatches)
{

    std::vector<unsigned> ignored;
    QList<QList<unsigned> > matched;

    bool ignore = false;

    for(int i=0; i < colorMatches.size(); i++){
        QList<unsigned> currentMatchList = colorMatches.at(i);
        unsigned max = currentMatchList.size();
        ignored.clear();
        for(unsigned i=0; i< max; i++)
        {
            ignore = false;
            foreach (unsigned x, ignored)
            {
                if((x == currentMatchList.at(i))){
                    ignore = true;
                    break;
                }
            }

            if(!ignore)
            {
                QList<unsigned> list;
                list.append(currentMatchList.at(i));

                for(unsigned j=i+1; j<max; j++)
                {
                    foreach (unsigned x, ignored)
                    {
                        if((x == currentMatchList.at(j))){
                            ignore = true;
                            break;
                        }
                    }

                    if(!ignore) {
                        double match = cv::matchShapes(contours[currentMatchList.at(i)],
                                contours[currentMatchList.at(j)],
                                CV_CONTOURS_MATCH_I2, 0);

                        if(match < shape_thress)
                        {
                            list.append(currentMatchList.at(j));

                            ignored.push_back(currentMatchList.at(j));
                            ignored.push_back(currentMatchList.at(i));
                        }

                    } else {
                        ignore=false;
                    }
                }
                matched.append(list);
            }
        }
    }
    return matched;
}

QList<QList<unsigned> > ImgProc::get_colorMatches(const double &color_tress,
                                                  const QString &path,
                                                  std::vector<std::vector<cv::Point> > contours)
{
    cv::Mat img = cv::imread(path.toStdString(), 1);
    cv::Mat tmpImg;
    cv::Rect ROI;
    std::vector<std::vector<cv::Point> > currentContour;
    QList<cv::MatND*> histograms;
    unsigned max = contours.size();
    std::vector<unsigned> ignored;
    QList<QList<unsigned> > matched;
    bool ignore = false;

    //Using 30 bins for hue and 32 for saturation
    int h_bins = 256; //int s_bins = 32;
    int histSize[] = { h_bins, h_bins,h_bins,h_bins };
    // hue varies from 0 to 256, saturation from 0 to 180
    float h_ranges[] = { 0, 256 };
    // float s_ranges[] = { 0, 180 };

    const float* ranges[] = { h_ranges, h_ranges,h_ranges,h_ranges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1,2,3 };

    for(unsigned i = 0; i< max; i++){

        ROI = cv::boundingRect(contours.at(i));
        currentContour = std::vector< std::vector<cv::Point> > (1,contours.at(i));
        cv::Mat crop;
        cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        cv::drawContours(mask, currentContour, -1, cv::Scalar(255), CV_FILLED);
        img.copyTo(crop,mask);
        tmpImg = crop(ROI);
        //cvtColor( tmpImg, tmpImg, CV_BGR2HSV );
        cv::MatND* curHis = new cv::MatND();
        calcHist( &tmpImg, 1, channels, cv::Mat(), *curHis, 2, histSize, ranges, true, false );
        normalize( *curHis, *curHis, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
        histograms.append(curHis);
    }


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
                    //THIS IS THE SIMPLE CHECK WHERE IT CHECKS THE RATIO BETWEEN THE HEIGHT OF
                    //THE minarearect AND THE RADIUS OF MINENCLOSING CIRCLE. THIS IS DONE BEFORE THE COLOR CHECK
                    float height1;
                    float height2;
                    float radius1;
                    float radius2;
                    cv::Point2f center;

                    //The following check is done get the heightest side of the minarea rect is this changes depending on rotation
                    if((cv::minAreaRect(contours.at(i)).size.height < cv::minAreaRect(contours.at(i)).size.width)){
                        height1 = cv::minAreaRect(contours.at(i)).size.height;
                    } else {
                        height1 = cv::minAreaRect(contours.at(i)).size.width;
                    }

                    if(cv::minAreaRect(contours.at(j)).size.height < cv::minAreaRect(contours.at(j)).size.width){
                        height2 = cv::minAreaRect(contours.at(j)).size.height;
                    } else {
                        height2 = cv::minAreaRect(contours.at(j)).size.width;
                    }

                    cv::minEnclosingCircle(contours.at(i),center,radius1);
                    cv::minEnclosingCircle(contours.at(j),center,radius2);

                    float test1 = height1/radius1;
                    float test2 = height2/radius2;
                    if(test1-test2 < 0.1 && test1-test2 > -0.1){
                        double match = cv::compareHist(*histograms.at(i),
                                                       *histograms.at(j),
                                                       CV_COMP_BHATTACHARYYA);

                        if(match < color_tress)
                        {  qDebug() << match;
                            qDebug() << color_tress;
                            list.append(j);

                            ignored.push_back(j);
                            ignored.push_back(i);
                        }
                    }
                } else ignore=false;
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
        return contours;
    }

    cvtAlphaToBinary(img, img_out);
    cv::findContours(img_out,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    std::vector<std::vector<cv::Point> >hull( contours.size() );
    for( unsigned int i = 0; i < contours.size(); i++ )
    {
        convexHull( cv::Mat(contours[i]), hull[i], false );
    }

    return hull;
}

std::vector<std::vector<cv::Point> > ImgProc::findContoursFromQImage(QImage QImg)
{
    cv::Mat img = QImage2Mat(QImg);
    cv::Mat img_out;

    std::vector<std::vector<cv::Point> > contours;

    if (! img.data )
    {
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


