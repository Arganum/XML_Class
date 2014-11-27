#ifndef ROADXML_H
#define ROADXML_H

#include <QXmlStreamReader>
#include "roadStructs.h"

class roadXML
{
public:
    roadXML();

    Bounds Boundary;
    std::vector<Way> Highways;

    bool readFirstParse(QIODevice *device);
    bool readSecondParse(QIODevice *device);

    void printHighways(uint _highwayNumber = 0); //0 print all
    void printWayNode(uint _highwayNumber = 0); //0 print all nodes with Longtitude, Latitude and ID
    void printWayNodeID(uint _highwayNumber = 0); //0 print all nodes ID

    void deleteNodeIDLists();

private:
    QXmlStreamReader xml;
    QXmlStreamReader xml0;

    void testStructs(bool use);
    void readOSMFirstParse();
    void readOSMSecondParse();
    void readBounds();
    void readNode();
    void readWay();
    void readNd(Way &_tempWay);
    void readTag(Way &_tempWay);
};

#endif // ROADXML_H
