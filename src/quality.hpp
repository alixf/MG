#ifndef QUALITY_H
#define QUALITY_H

#include <iostream>
#include <QVector3D>
#include <QPoint>

class Quality
{
public :
    static float getAspectRatio1 (const std::vector<Face>& faces, float &ratioMean, float &ratioSD, float &ratioMin, float &ratioMax);
    static float getAspectRatio2 (const std::vector<Face>& faces, float &ratioMean, float &ratioSD, float &ratioMin, float &ratioMax);
    static float getAspectRatio3 (const std::vector<Face>& faces, float &ratioMean, float &ratioSD, float &ratioMin, float &ratioMax);
    static float getDegree(const std::vector<Vertex*> &vertices, float& degreeMean, float& degreeSD, float& degreeMin, float& degreeMax);

    static void contours (std::vector<Edge>& edges, std::vector<Edge>& f, std::vector<Edge>& nonVariety);
    static float distancePointToPoint(const QVector3D &pos1, const QVector3D &pos2);
    static QVector3D vectorPointToPoint(const QVector3D &p1, const QVector3D &p2);
    static float distancePointToRay(const QVector3D& origin, const QVector3D& dir, const QVector3D& point);
    static void extractContours (std::vector<Edge*>& edges, std::vector<Edge*>& f, std::vector<Edge>& nonVariety);
    static int nbHole (std::vector<Edge *> &edges);

   /* static Quality* const getQualityObject() {
        if(!Quality::thisRef){
            Quality::thisRef = new Quality();
        }
        return const_cast<Quality*>(Quality::thisRef);
    }

    void destroyQuality() { if(Quality::thisRef) delete Quality::thisRef; }

private :
    Quality();
    static Quality* thisRef;*/
};

#endif // QUALITY_H
