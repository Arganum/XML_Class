#include "roadXML.h"
#include "roadStructs.h"

#include <iostream>

roadXML::roadXML()
{
}

bool roadXML::readFirstParse(QIODevice *device)
{
    //roadXML::testStructs(false);
    xml.setDevice(device);

    if (xml.readNextStartElement())
    {
        if (xml.name() == "osm" && xml.attributes().value("version") == "0.6")
        {
            readOSMFirstParse();
            std::cout << "First Parse Through, Way nodeIDList for each Way is populated." << std::endl;
        }
        else
            xml.raiseError("The file is not an OSM version 0.6 file.");
    }

    bool error = !xml.error();
    xml.clear();
    return error;
}

bool roadXML::readSecondParse(QIODevice *device)
{
    xml0.setDevice(device);

    if (xml0.readNextStartElement())
    {
        //std::cout << "FAIL" << std::endl;
        if (xml0.name() == "osm" && xml0.attributes().value("version") == "0.6")
        {
            readOSMSecondParse();
            //deleteNodeIDLists();
            std::cout << "Second Parse Through, Way wayNodeList for each Way is populated with Node(s)." << std::endl;
        }
        else
            xml0.raiseError("The file is not an OSM version 0.6 file.");
    }

    bool error = !xml0.error();
    xml0.clear();
    return error;
}

void roadXML::readOSMFirstParse()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "osm");

    while (xml.readNextStartElement())
    {
        if (xml.name() == "way")
            readWay();
        else
            xml.skipCurrentElement();
    }
}

void roadXML::readOSMSecondParse()
{
    Q_ASSERT(xml0.isStartElement() && xml0.name() == "osm");

    while (xml0.readNextStartElement())
    {
        if (xml0.name() == "bounds")
            readBounds();
        else if (xml0.name() == "node")
            readNode();
        else
            xml0.skipCurrentElement();
    }
}

void roadXML::readBounds()
{
    std::cout << std::endl;
    std::cout << "Bounds" << std::endl;

    Boundary.minLatitude = xml0.attributes().value("minlat").toDouble();
    Boundary.minLongitude = xml0.attributes().value("minlon").toDouble();
    Boundary.maxLatitude = xml0.attributes().value("maxlat").toDouble();
    Boundary.maxLongitude = xml0.attributes().value("maxlon").toDouble();

    std::cout.precision(15);
    std::cout << "Bounds Min Latitude: " << Boundary.minLatitude << std::endl;
    std::cout << "Bounds Min Longitude: " << Boundary.minLongitude << std::endl;
    std::cout << "Bounds Max Latitude: " << Boundary.maxLatitude << std::endl;
    std::cout << "Bounds Max Longitude: " << Boundary.maxLongitude << std::endl;
    std::cout << std::endl;

    while (xml0.readNextStartElement())
    {
        if (xml0.name() == "node")
            readNode();
        else
            xml0.skipCurrentElement();
    }
}

void roadXML::readNode()
{
    Q_ASSERT(xml0.isStartElement() && xml0.name() == "node");

    for (uint i = 0; i < Highways.size(); i++)
    {
        for (uint j = 0; j < Highways[i].nodeIDList.size(); j++)
        {
            if (xml0.attributes().value("id").toULong() == Highways[i].nodeIDList[j])
            {              
                struct Node tempNode;

                tempNode.nodeID = xml0.attributes().value("id").toULong();
                tempNode.nodeLatitude = xml0.attributes().value("lat").toDouble();
                tempNode.nodeLongitude = xml0.attributes().value("lon").toDouble();

                Highways[i].wayNodeList.push_back(tempNode);
            }
        }
    }

    while (xml0.readNextStartElement())
    {
        if (xml0.name() == "node")
            readNode();
        else
            xml0.skipCurrentElement();
    }
}

void roadXML::readWay()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "way");

    struct Way tempWay;
    //std::cout << tempWay.wayName << std::endl;
    //std::cout << tempWay.wayHighwayType << std::endl;
    //if ( tempWay.aHighway == false )
        //std::cout << "False" << std::endl;
    //tempWay.aHighway = false;
    //tempWay.wayName = "None";
    //tempWay.wayHighwayType = "None";

    while (xml.readNextStartElement())
    {
        if (xml.name() == "way")
            readWay();
        else if (xml.name() == "nd")
            readNd(tempWay);
        else if (xml.name() == "tag")
            readTag(tempWay);
        else
            xml.skipCurrentElement();
    }

    if (tempWay.aHighway == true && tempWay.wayHighwayType != "service")
    {
        //tempWay.nodeIDList.clear();
        Highways.push_back(tempWay);
    }
}

void roadXML::readNd(Way &_tempWay)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "nd");

    _tempWay.nodeIDList.push_back( xml.attributes().value("ref").toULong() );

    while (xml.readNextStartElement())
    {
        if (xml.name() == "way")
            readWay();
        else if (xml.name() == "nd")
            readNd(_tempWay);
        else if (xml.name() == "tag")
            readTag(_tempWay);
        else
            xml.skipCurrentElement();
    }
}

void roadXML::readTag(Way &_tempWay)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "tag");
    //std::cout << "Read Tag" << std::endl;

    if (xml.attributes().value("k") == "highway" )
    {
        _tempWay.aHighway = true;
        _tempWay.wayHighwayType = xml.attributes().value("v").toUtf8().constData();
        //std::cout << _tempWay.wayHighwayType << std::endl;
    }

    if (xml.attributes().value("k") == "name" )
    {
        _tempWay.wayName = xml.attributes().value("v").toUtf8().constData();
        //std::cout << _tempWay.wayName << std::endl;
    }

    while (xml.readNextStartElement())
    {
        if (xml.name() == "way")
            readWay();
        else if (xml.name() == "nd")
            readNd(_tempWay);
        else if (xml.name() == "tag")
            readTag(_tempWay);
        else
            xml.skipCurrentElement();
    }
}

void roadXML::deleteNodeIDLists()
{
    for (uint i = 0; i < Highways.size(); i++)
    {
        Highways[i].nodeIDList.clear();
    }
}

void roadXML::printHighways(uint _highwayNumber)
{
    if(_highwayNumber == 0)
    {
        for (uint i = 0; i < Highways.size(); i++)
        {
            std::cout << "Way: " << i << std::endl;
            std::cout << "Way Name: " << Highways[i].wayName << std::endl;
            std::cout << "Way Type: " << Highways[i].wayHighwayType << std::endl;
            std::cout << "Way Number of Nodes: " << Highways[i].wayNodeList.size() << std::endl << std::endl;
        }
    }
    else if (Highways.size() != 0)
    {
        std::cout << "Way: " << _highwayNumber << std::endl;
        std::cout << "Way Name: " << Highways[_highwayNumber].wayName << std::endl;
        std::cout << "Way Type: " << Highways[_highwayNumber].wayHighwayType << std::endl;
        std::cout << "Way Number of Nodes: " << Highways[_highwayNumber].wayNodeList.size() << std::endl << std::endl;
    }
}

void roadXML::printWayNode(uint _highwayNumber)
{
    if(_highwayNumber == 0)
    {
        for (uint i = 0; i < Highways.size(); i++)
        {
            std::cout << "Way: " << i << std::endl;
            std::cout << "Way Name: " << Highways[i].wayName << std::endl;
            std::cout << "Way Type: " << Highways[i].wayHighwayType << std::endl;
            std::cout << "Way Nodes: " << std::endl;
            for (uint j = 0; j < Highways[i].wayNodeList.size(); j++)
            {
                std::cout.precision(15);
                std::cout << "Node: " << j << "   ID: " << Highways[i].wayNodeList[j].nodeID;
                std::cout << "   Lat: " << Highways[i].wayNodeList[j].nodeLatitude;
                std::cout << "   Lon: " << Highways[i].wayNodeList[j].nodeLongitude;
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
    else if (Highways.size() != 0)
    {
        std::cout << "Way: " << _highwayNumber << std::endl;
        std::cout << "Way Name: " << Highways[_highwayNumber].wayName << std::endl;
        std::cout << "Way Type: " << Highways[_highwayNumber].wayHighwayType << std::endl;
        std::cout << "Way Nodes: " << std::endl;
        for (uint j = 0; j < Highways[_highwayNumber].wayNodeList.size(); j++)
        {
            std::cout.precision(15);
            std::cout << "Node: " << j << "   ID: " << Highways[_highwayNumber].wayNodeList[j].nodeID;
            std::cout << "   Lat: " << Highways[_highwayNumber].wayNodeList[j].nodeLatitude;
            std::cout << "   Lon: " << Highways[_highwayNumber].wayNodeList[j].nodeLongitude;
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void roadXML::printWayNodeID(uint _highwayNumber)
{
    if(_highwayNumber == 0)
    {
        for (uint i = 0; i < Highways.size(); i++)
        {
            std::cout << "Way: " << i << std::endl;
            std::cout << "Way Name: " << Highways[i].wayName << std::endl;
            std::cout << "Way Type: " << Highways[i].wayHighwayType << std::endl;
            std::cout << "Way Nodes: " << std::endl;
            for (uint j = 0; j < Highways[i].nodeIDList.size(); j++)
            {
                std::cout << "Node: " << j << "   ID: " << Highways[i].nodeIDList[j] << std::endl;
            }
            std::cout << std::endl;
        }
    }
    else if (Highways.size() != 0)
    {
        std::cout << "Way: " << _highwayNumber << std::endl;
        std::cout << "Way Name: " << Highways[_highwayNumber].wayName << std::endl;
        std::cout << "Way Type: " << Highways[_highwayNumber].wayHighwayType << std::endl;
        std::cout << "Way Nodes: " << std::endl;
        for (uint j = 0; j < Highways[_highwayNumber].nodeIDList.size(); j++)
        {
            std::cout << "Node: " << j << "   ID: " << Highways[_highwayNumber].nodeIDList[j] << std::endl;
        }
        std::cout << std::endl;
    }
}

/*
void roadXML::readOSM()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "osm");

    while (xml.readNextStartElement())
    {
        if (xml.name() == "node")
            readNode();
        else if (xml.name() == "way")
            readWay();
        else
            xml.skipCurrentElement();
    }
}

void roadXML::readNode()
{
    std::cout << "Hej1" << std::endl;
    Q_ASSERT(xml.isStartElement() && xml.name() == "node");
    std::cout << "Hej2" << std::endl;

    while (xml.readNextStartElement())
    {
        if (xml.name() == "node")
            readNode();
        else if (xml.name() == "way")
            readWay();
        else
            xml.skipCurrentElement();
    }
}

void roadXML::readWay()
{
    std::cout << "Hej3" << std::endl;
    Q_ASSERT(xml.isStartElement() && xml.name() == "way");
    std::cout << "Hej4" << std::endl;

    while (xml.readNextStartElement())
    {
        if (xml.name() == "node")
            readNode();
        else if (xml.name() == "way")
            readWay();
        else
            xml.skipCurrentElement();
    }
}
*/

void roadXML::testStructs(bool use)
{
    if (!use)
    {
        return;
    }

    struct Way tempWay;
    struct Node tempNode;
    tempNode.nodeID = 1;
    tempNode.nodeLatitude = 10.5;
    tempNode.nodeLongitude = 10.5;
    tempWay.wayNodeList.push_back(tempNode);
    tempWay.wayName = "Grøn";
    tempWay.wayHighwayType = "Unclassy";
    Highways.push_back(tempWay);

    std::string tempString;

    tempString = Highways[0].wayName;
    //QString tempString = Highways[0].wayName;
    int tempInt = Highways[0].wayNodeList[0].nodeID;

    std::cout << "Highway 0 wayName is: " << tempString << std::endl;
    //std::cout << "Highway 0 wayName is: " << tempString.toStdString() << std::endl;
    std::cout << "Highway 0 wayNodeList 0 nodeID is: " << tempInt << std::endl;
    std::cout << "Highway 0 wayNodeList.size() is: " << Highways[0].wayNodeList.size() << std::endl;

    return;
}

