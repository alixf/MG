#include "vefmodel.hpp"
#include "octree.hpp"
#include "math.h"
#include "quality.hpp"
#include <cmath>
#include <limits>

void analyseArray(std::vector<float>& data, float& mean, float& sd, float& _min, float& _max)
{
    mean = 0.0;
    sd = 0.0;
    _max = std::numeric_limits<float>::min();
    _min = std::numeric_limits<float>::max();

    for(std::vector<float>::iterator it = data.begin(); it != data.end(); ++it)
    {
        mean += *it;
        _min = fmin(*it, _min);
        _max = fmax(*it, _max);
    }
    mean = mean / data.size();

    for(std::vector<float>::iterator it = data.begin(); it != data.end(); ++it)
        sd += (*it - mean) * (*it - mean);
    sd = sqrt(sd / data.size());
}

float Quality::getAspectRatio1(const std::vector<Face>& faces, float& ratioMean, float& ratioSD, float& ratioMin, float& ratioMax)
{
    QVector3D p1, p2, p3;
    float distp1p2, distp1p3, distp2p3;
    float distBase, distPoint;
    QVector3D vecBase;

    std::vector<float> ratios;
    ratios.resize(faces.size());

    // Extract all ratios into data
    for (unsigned int i = 0; i < faces.size(); ++i)
    {
        if (faces[i].vertices.size() == 3)
        {
            p1 = QVector3D(faces[i].vertices[0]->x, faces[i].vertices[0]->y, faces[i].vertices[0]->z);
            p2 = QVector3D(faces[i].vertices[1]->x, faces[i].vertices[1]->y, faces[i].vertices[1]->z);
            p3 = QVector3D(faces[i].vertices[2]->x, faces[i].vertices[2]->y, faces[i].vertices[2]->z);

            distp1p2 = distancePointToPoint(p1, p2);
            distp1p3 = distancePointToPoint(p1, p3);
            distp2p3 = distancePointToPoint(p2, p3);

            if (distp1p2 > distp1p3 && distp1p2 > distp2p3)
            {
                    distBase = distp1p2;
                    vecBase = vectorPointToPoint(p1, p2);
                    distPoint = Quality::distancePointToRay(p1, vecBase, p3);
            }
            else if (distp1p3 > distp1p2 && distp1p3 > distp2p3)
            {
                    distBase = distp1p3;
                    vecBase = vectorPointToPoint(p1, p3);
                    distPoint = Quality::distancePointToRay(p3, vecBase, p2);
            }
            else
            {
                distBase = distp1p3;
                vecBase = vectorPointToPoint(p1, p3);
                distPoint = Quality::distancePointToRay(p3, vecBase, p2);
            }

            ratios[i] = (distPoint / distBase);
        }
    }

    analyseArray(ratios, ratioMean, ratioSD, ratioMin, ratioMax);
    return ratioMean;
}

float Quality::getAspectRatio2(const std::vector<Face> &faces, float& ratioMean, float& ratioSD, float& ratioMin, float& ratioMax)
{
    QVector3D p1,p2,p3;
    float distp1p2, distp1p3, distp2p3;
    float s;

    std::vector<float> ratios;
    ratios.resize(faces.size());

    for (unsigned int i = 0; i < faces.size(); ++i)
    {
        if (faces[i].vertices.size() == 3)
        {
            p1 = QVector3D(faces[i].vertices[0]->x, faces[i].vertices[0]->y, faces[i].vertices[0]->z);
            p2 = QVector3D(faces[i].vertices[1]->x, faces[i].vertices[1]->y, faces[i].vertices[1]->z);
            p3 = QVector3D(faces[i].vertices[2]->x, faces[i].vertices[2]->y, faces[i].vertices[2]->z);

            distp1p2 = distancePointToPoint(p1, p2);
            distp1p3 = distancePointToPoint(p1, p3);
            distp2p3 = distancePointToPoint(p2, p3);

            s = (distp1p2 + distp1p3 + distp2p3) / 2.f;
            ratios[i] = (distp1p2 * distp1p3 * distp2p3) / (8.f * (s - distp1p2) * (s - distp1p3) * (s - distp2p3));
        }
    }

    analyseArray(ratios, ratioMean, ratioSD, ratioMin, ratioMax);
    return ratioMean;
}

float Quality::getAspectRatio3(const std::vector<Face> &faces, float& ratioMean, float& ratioSD, float& ratioMin, float& ratioMax)
{

    QVector3D p1;
    QVector3D p2;
    QVector3D p3;

    float distp1p2; //a
    float distp1p3; //b
    float distp2p3; //c

    float edgeMax = 0.f;
    float edgeMin = 0.f;

    std::vector<float> ratios;
    ratios.resize(faces.size());

    for (unsigned int i = 0; i < faces.size(); ++i)
    {
        if (faces[i].vertices.size() == 3)
        {
            p1 = QVector3D(faces[i].vertices[0]->x, faces[i].vertices[0]->y, faces[i].vertices[0]->z);
            p2 = QVector3D(faces[i].vertices[1]->x, faces[i].vertices[1]->y, faces[i].vertices[1]->z);
            p3 = QVector3D(faces[i].vertices[2]->x, faces[i].vertices[2]->y, faces[i].vertices[2]->z);

            distp1p2 = distancePointToPoint(p1, p2);
            distp1p3 = distancePointToPoint(p1, p3);
            distp2p3 = distancePointToPoint(p2, p3);

            edgeMax = std::max(std::max(distp1p2, distp1p3), distp2p3);
            edgeMin = std::min(std::min(distp1p2, distp1p3), distp2p3);

            ratios[i] = edgeMax / edgeMin;
        }
        else
        {
            std::cerr << "face sans 3 aretes" << std::endl;
        }
    }

    analyseArray(ratios, ratioMean, ratioSD, ratioMin, ratioMax);
    return ratioMean;
}

float Quality::getDegree(const std::vector<Vertex*> &vertices, float& degreeMean, float& degreeSD, float& degreeMin, float& degreeMax)
{
    std::vector<float> degrees;
    degrees.resize(vertices.size());

    for (unsigned int i = 0; i < vertices.size(); ++i)
        degrees[i] = vertices[i]->edges.size();

    analyseArray(degrees, degreeMean, degreeSD, degreeMin, degreeMax);
    return degreeMean;
}

Edge* getNextEdge (std::vector<Edge*> edges, Edge* edge, std::vector<Edge*>& edgesTraited)
{
     for (unsigned int i = 0; i < edges.size(); ++i)
     {
         if(std::find(edgesTraited.begin(), edgesTraited.end(), edges[i]) == edgesTraited.end()) // If edge is not in the visited list
         {
             if(edge != edges[i])
             {
                 if (edge->v2 == edges[i]->v1 || edge->v2 == edges[i]->v2 || edge->v1 == edges[i]->v1 || edge->v1 == edges[i]->v2)
                     return edges[i];
             }
         }
     }
     return NULL;
}

int Quality::nbHole(std::vector<Edge*>& edges)
{
    int cmp = 0;

    std::vector<Edge*> e;
    Edge* nextEdge;

    for (unsigned int i = 1; i < edges.size(); ++i)
    {
        if(std::find(e.begin(), e.end(), edges[i]) == e.end()) // If edge is not in the visited list
        {
            nextEdge = getNextEdge(edges, edges[i], e);

            while (nextEdge != NULL)
            {
                e.push_back(nextEdge);
                nextEdge = getNextEdge(edges, nextEdge, e);
            }

            cmp++;
        }
    }

    return cmp;
}

void Quality::extractContours (std::vector<Edge*>& edges, std::vector<Edge*>& f, std::vector<Edge>& nonVariety) {

    for (unsigned int i = 0; i < edges.size(); ++i)
    {
        if (edges[i]->faces.size() < 2)
        {
            f.push_back(edges[i]);
            edges[i]->v1->r = 1.f;
            edges[i]->v1->g = edges[i]->v1->b = 0.f;
        }
        else if (edges[i]->faces.size() > 2)
        {
            nonVariety.push_back(*(edges[i]));
        }
    }
}

float Quality::distancePointToPoint(const QVector3D &pos1, const QVector3D &pos2){
    return sqrt((pos1.x() - pos2.x()) * (pos1.x() - pos2.x()) + (pos1.y() - pos2.y()) * (pos1.y() - pos2.y()) + (pos1.z() - pos2.z()) * (pos1.z() - pos2.z()));

}

QVector3D Quality::vectorPointToPoint(const QVector3D &p1, const QVector3D &p2)  {

    return QVector3D(p2.x() - p1.x(), p2.y() - p1.y(), p2.z() - p1.z());
}

float Quality::distancePointToRay(const QVector3D& origin, const QVector3D& dir, const QVector3D& point)  {
    QVector3D dirToPoint = QVector3D(point.x() - origin.x(), point.y() - origin.y(), point.z() - origin.z());

    float scalarP = dir.x() * dirToPoint.x() +  dir.y() * dirToPoint.y() + dir.z() * dirToPoint.z();
    float norm = sqrt(dir.x() * dir.x() + dir.y() * dir.y() + dir.z() * dir.z()) *
                 sqrt(dirToPoint.x() * dirToPoint.x() + dirToPoint.y() * dirToPoint.y() + dirToPoint.z() * dirToPoint.z());

    float angle =  acos(scalarP / norm);

    return sin(angle) * Quality::distancePointToPoint(origin, point);
}
