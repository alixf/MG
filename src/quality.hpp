#ifndef QUALITY_H
#define QUALITY_H

#include <iostream>
#include <QVector3D>
#include <QPoint>

class Quality
{
public :
    static float AspectRatio1 (const std::vector<Face>& faces);
    static float AspectRatio2 (const std::vector<Face>& faces);
    static float AspectRatio3 (const std::vector<Face>& faces) ;
    static float maximum(const float x, const float y, const float z);
    static float minimum(const float x, const float y, const float z);
    static void contours (std::vector<Edge>& edges, std::vector<Edge>& f, std::vector<Edge>& nonVariety);
    static float distancePointToPoint(const QVector3D &pos1, const QVector3D &pos2);
    static QVector3D vectorPointToPoint(const QVector3D &p1, const QVector3D &p2);
    static float distancePointToRay(const QVector3D& origin, const QVector3D& dir, const QVector3D& point);
    static void extractContours (const std::vector<Edge>& edges, std::vector<Edge>& f, std::vector<Edge>& nonVariety);

    static Quality* const getQualityObject() {
        if(!thisRef){
            Quality::thisRef = new Quality();
        }
        return const_cast<Quality*>(Quality::thisRef);
    }

    void destroyQuality() { if(Quality::thisRef) delete Quality::thisRef; }

private :
    Quality();
    static Quality* thisRef;
};

#endif // QUALITY_H
