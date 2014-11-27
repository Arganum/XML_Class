#ifndef ROADPAINTER_H
#define ROADPAINTER_H

#include <QImage>
#include <QPainter>
#include <QFile>
#include <iostream>
#include <math.h>
#include "roadStructs.h"

class roadPainter
{
public:
    roadPainter();

    void intiImage(Bounds _bound);
    void intiPainter();
    void endPainter();
    void drawLines(Bounds _bound, std::vector<Way> _ways);

    QImage readImage();
    void saveImage(QFile *device);


    //QImage::Format_RGB32
private:
    QImage image;
    QPainter painter;

    QPointF convLatLon(double _longitude, double _latitude);
    QPointF normalizePoint(QPointF _pointNorm, QPointF _point);
    QColor findRoadColor(double _totalRoad, double _currRoad);

};

#endif // ROADPAINTER_H
