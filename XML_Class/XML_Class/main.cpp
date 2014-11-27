#include <QCoreApplication>
#include <QFile>
#include <iostream>

#include <roadXML.h>
#include <roadPainter.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    roadXML reader;

    QFile file("D:/Dropbox/Litteratur/OSM/map.osm");

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cout << "Cannot read file" << std::endl;
        return a.exec();
    }

    if (!reader.readFirstParse(&file))
    {
        std::cout << "First Parse error in file" << std::endl;
    } else {
        std::cout << "First Parse file successfully read" << std::endl;
    }

    file.close();

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cout << "Cannot read file" << std::endl;
        return a.exec();
    }

    if (!reader.readSecondParse(&file))
    {
        std::cout << "Second Parse error in file" << std::endl;
    } else {
        std::cout << "Second Parse file successfully read" << std::endl;
    }

    std::cout << std::endl;

    reader.printWayNodeID();
    reader.deleteNodeIDLists();

    std::cout << "nodeIDLists for each Way deleted" << std::endl << std::endl;

    reader.printHighways();
    reader.printWayNode();

    roadPainter painter;

    painter.intiImage(reader.Boundary);
    painter.intiPainter();
    painter.drawLines(reader.Boundary,reader.Highways);
    painter.endPainter();

    QImage image = painter.readImage();

    QFile roadMap("D:/Dropbox/Litteratur/OSM/RoadMap.png");

    painter.saveImage(&roadMap);

    file.close();
    roadMap.close();

    std::cout << "All Files Closed" << std::endl;

    long double x, y, x1, y1, x2, y2, param1, param2, dec1, dec2, dec3, dec4;
    dec1 = 10.3649600;
    dec2 = 55.3954200;
    param1 = dec1 * M_PI / 180;
    param2 = dec2 * M_PI / 180;
    x1 = ( M_PI / 180 ) * 6378137.0 * cos( atan( 0.996647189328169 * tan( param1 ) ) ); // Wiki Length_of_a_degree_of_longitude
    y1 = 111132.954 - 559.822 * cos( 2 * param2 ) + 1.175 * cos( 4 * param2 );

    dec3 = 10.3685200;
    dec4 = 55.3967400;
    param1 = dec3 * M_PI / 180;
    param2 = dec4 * M_PI / 180;
    x2 = ( M_PI / 180 ) * 6378137.0 * cos( atan( 0.996647189328169 * tan( param1 ) ) ); // Wiki Length_of_a_degree_of_longitude
    y2 = 111132.954 - 559.822 * cos( 2 * param2 ) + 1.175 * cos( 4 * param2 );

    x = x2 - x1;
    y = y2 - y1;

    std::cout.precision(15);
    std::cout << std::endl;
    std::cout << "X is: " << x << " At: " << dec1 << " Degrees" << std::endl;
    std::cout << "Y is: " << y << " At: " << dec2 << " Degrees" << std::endl;
    /*
    double result, param;
    param = 90;
    result = atan( tan( param * M_PI / 180 ) );
    std::cout << result << std::endl;
    //*/

    return a.exec();
}
