#include "roadPainter.h"

roadPainter::roadPainter()
{
}

void roadPainter::intiImage(Bounds _bound)
{
    QPointF minSize, maxSize, nSize;
    minSize = convLatLon(10.3649600, 55.3954200);
    maxSize = convLatLon(10.3685200, 55.3967400);

    nSize = normalizePoint(maxSize, minSize);

    std::cout << "Size: " << minSize.x() << " , " << minSize.y() << std::endl;
    std::cout << "Size: " << maxSize.x() << " , " << maxSize.y() << std::endl;
    std::cout << "Size: " << floor( nSize.x() + 0.5 ) << " , " << floor( nSize.y() + 0.5 ) << std::endl;
    //QImage tempImage(size.x() * MAP_SCALE, size.y() * MAP_SCALE, QImage::Format_RGB32);

    QImage tempImage( 1000, 1000, QImage::Format_RGB32);
    tempImage.fill(Qt::black);
    image = tempImage;
}

void roadPainter::intiPainter()
{
    painter.begin(&image);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::white);
}

void roadPainter::endPainter()
{
    painter.end();
    //*
    QColor test = findRoadColor(5,1);
    std::cout << "Color: 1 " << test.red() << " , " << test.green() << " , " << test.blue() << std::endl;
    test = findRoadColor(5,2);
    std::cout << "Color: 2 " << test.red() << " , " << test.green() << " , " << test.blue() << std::endl;
    test = findRoadColor(5,3);
    std::cout << "Color: 3 " << test.red() << " , " << test.green() << " , " << test.blue() << std::endl;
    test = findRoadColor(5,4);
    std::cout << "Color: 4 " << test.red() << " , " << test.green() << " , " << test.blue() << std::endl;
    test = findRoadColor(5,5);
    std::cout << "Color: 5 " << test.red() << " , " << test.green() << " , " << test.blue() << std::endl;
    //*/
}

void roadPainter::drawLines(Bounds _bound, std::vector<Way> _ways)
{
    QPointF p1, p2, p3, p4, p5;
    QLineF line, line1, line2;

    p5 = convLatLon(_bound.minLongitude , _bound.minLatitude);

    p1 = convLatLon(_ways[2].wayNodeList[0].nodeLongitude, _ways[2].wayNodeList[0].nodeLatitude);
    p2 = convLatLon(_ways[2].wayNodeList[1].nodeLongitude, _ways[2].wayNodeList[1].nodeLatitude);

    p3 = normalizePoint(p1, p5);
    p4 = normalizePoint(p2, p5);

    std::cout << "NP1 : " << p3.x() << " , " << p3.y() << std::endl;
    std::cout << "NP2 : " << p4.x() << " , " << p4.y() << std::endl;
    std::cout << "P1 : " << p1.x() << " , " << p1.y() << std::endl;
    std::cout << "P2 : " << p2.x() << " , " << p2.y() << std::endl;
    std::cout << std::endl;

    line.setPoints(p3,p4);
    line1.setLine(0,0,0,image.height());
    QPointF pIntersectionPoint;

    int test = line.intersect(line1,&pIntersectionPoint);

    if ( test == QLineF::BoundedIntersection )
    {
        std::cout << "Jo Dude!!!: " << test << std::endl;
    }

    std::cout << "InterPoint: " << pIntersectionPoint.x() << " , " << pIntersectionPoint.y() << std::endl;

    line2.setPoints(pIntersectionPoint, p4);
    /*
    line.setLine(1,0,5,4);
    line1.setLine(0,0,5,0);
    QPointF pIntersectionPoint;

    int test = line.intersect(line1,&pIntersectionPoint);

    if ( test == QLineF::BoundedIntersection )
    {
        std::cout << "Jo Dude!!!: " << test << std::endl;
    }

    std::cout << "InterPoint: " << pIntersectionPoint.x() << " , " << pIntersectionPoint.y() << std::endl;

    std::cout << "Slope: " << line.dy()/line.dx() << std::endl;
    std::cout << "Slope: " << line.dx()*p4.x() << " , " << line.dy()*p4.y() << std::endl;
    //*/
    //Highways[_highwayNumber].wayNodeList[j]
    //_ways[2].wayNodeList[0];

    painter.drawLine(line2);
}

QPointF roadPainter::normalizePoint(QPointF _pointNorm, QPointF _point)
{
    QPointF tempPoint;

    tempPoint.setX( _pointNorm.x() - _point.x() );
    tempPoint.setY( _pointNorm.y() - _point.y() );

    return tempPoint;
}

QPointF roadPainter::convLatLon(double _longitude, double _latitude)
{
    double x, y, degX, degY;
    degX = _longitude * M_PI / 180;
    degY = _latitude * M_PI / 180;
    x = ( M_PI / 180 ) * 6378137.0 * cos( atan( 0.996647189328169 * tan( degX ) ) ); // Wiki Length_of_a_degree_of_longitude
    y = 111132.954 - 559.822 * cos( 2 * degY ) + 1.175 * cos( 4 * degY );
    //Lat = Y Long = X
    QPointF tempPoint;
    tempPoint.setX( x * MAP_SCALE );
    tempPoint.setY( y * MAP_SCALE );
    return tempPoint;
}

QImage roadPainter::readImage()
{
    return image;
}

QColor roadPainter::findRoadColor(double _totalRoad, double _currRoad)
{
    //double value = MAX_ROAD/_totalRoad * _currRoad;
    //double thress = (value)/(MAX_ROAD);

    double thress = (_currRoad)/(_totalRoad);

    uchar r = 0;
    uchar g = 0;
    uchar b = 0;
    int sat = 255;

    if(thress <= 0.2)
    {
        b = sat;
        g = uchar(thress/0.2 * sat);

        return QColor(r,g,b);
    }
    else if(thress < 0.399)
    {
        g = sat;
        b = uchar(sat-(thress-0.4)/0.2 * sat);

        return QColor(r,g,b);
    }
    else if(thress < 0.599)
    {
        g = sat;
        r = uchar((thress-0.6)/0.2 * sat);

        return QColor(r,g,b);
    }
    else if(thress < 0.799)
    {
        r = sat;
        g = sat - uchar((thress-0.8)/0.2 * sat);

        return QColor(r,g,b);
    }
    else if(thress < 0.999)
    {
        r = sat;
        b = uchar((thress-1)/0.2 * sat);

        return QColor(r,g,b);
    }
    else if(thress >= 1)
    {
        r = sat;
        b = sat;
        return QColor(r,g,b);

    } else return QColor(255,255,255);
}

void roadPainter::saveImage(QFile *device)
{
    if (!image.save(device)) //OBS! Quality kan sættes til 100, default settings -1
        std::cout << "Cannot save map" << std::endl;
}
