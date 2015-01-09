#include "vefmodel.hpp"
#include "octree.hpp"
#include "math.h"
#include "quality.hpp"

float Quality::AspectRatio1(const std::vector<Face>& faces) {

    QVector3D p1;
    QVector3D p2;
    QVector3D p3;

    float distp1p2;
    float distp1p3;
    float distp2p3;

    float distBase;
    float distPoint;
    QVector3D vecBase;

    float ratio = 0;

    for (unsigned int i = 0; i < faces.size(); ++i) {
        if (faces[i].vertices.size() == 3) {
            p1 = QVector3D(faces[i].vertices[0]->x, faces[i].vertices[0]->y, faces[i].vertices[0]->z);
            p2 = QVector3D(faces[i].vertices[1]->x, faces[i].vertices[1]->y, faces[i].vertices[1]->z);
            p3 = QVector3D(faces[i].vertices[2]->x, faces[i].vertices[2]->y, faces[i].vertices[2]->z);

            distp1p2 = distancePointToPoint(p1, p2);
            distp1p3 = distancePointToPoint(p1, p3);
            distp2p3 = distancePointToPoint(p2, p3);

            if (distp1p2 > distp1p3 && distp1p2 > distp2p3) {
                    distBase = distp1p2;
                    vecBase = vectorPointToPoint(p1, p2);
                    distPoint = Quality::distancePointToRay(p1, vecBase, p3);
            }
            else if (distp1p3 > distp1p2 && distp1p3 > distp2p3){
                    distBase = distp1p3;
                    vecBase = vectorPointToPoint(p1, p3);
                    distPoint = Quality::distancePointToRay(p3, vecBase, p2);
            }
            else {
                distBase = distp1p3;
                vecBase = vectorPointToPoint(p1, p3);
                distPoint = Quality::distancePointToRay(p3, vecBase, p2);
            }

	    ratio += (distPoint / distBase);
        }
    }

    ratio = ratio / faces.size();

    return ratio;
}

float Quality::AspectRatio2(const std::vector<Face> &faces) {

    QVector3D p1;
    QVector3D p2;
    QVector3D p3;

    float distp1p2; //a
    float distp1p3; //b
    float distp2p3; //c


    float s, AR;
    float ratio = 0;

    for (unsigned int i = 0; i < faces.size(); ++i) {
        if (faces[i].vertices.size() == 3) {
            p1 = QVector3D(faces[i].vertices[0]->x, faces[i].vertices[0]->y, faces[i].vertices[0]->z);
            p2 = QVector3D(faces[i].vertices[1]->x, faces[i].vertices[1]->y, faces[i].vertices[1]->z);
            p3 = QVector3D(faces[i].vertices[2]->x, faces[i].vertices[2]->y, faces[i].vertices[2]->z);

            distp1p2 = distancePointToPoint(p1, p2);
            distp1p3 = distancePointToPoint(p1, p3);
            distp2p3 = distancePointToPoint(p2, p3);

            s = (distp1p2 + distp1p3 + distp2p3) / 2.f;
            AR =  (distp1p2 * distp1p3 * distp2p3) / (8.f * (s - distp1p2) * (s - distp1p3) * (s - distp2p3));
            ratio += AR;

        }
    }

    return ratio / faces.size();
}

float Quality::AspectRatio3(const std::vector<Face> &faces)  {

    QVector3D p1;
    QVector3D p2;
    QVector3D p3;

    float distp1p2; //a
    float distp1p3; //b
    float distp2p3; //c

    float edgeMax = 0.0f;
    float edgeMin = 0.0f;

    float ratio = 0;

    for (unsigned int i = 0; i < faces.size(); ++i) {
        if (faces[i].vertices.size() == 3) {
            p1 = QVector3D(faces[i].vertices[0]->x, faces[i].vertices[0]->y, faces[i].vertices[0]->z);
            p2 = QVector3D(faces[i].vertices[1]->x, faces[i].vertices[1]->y, faces[i].vertices[1]->z);
            p3 = QVector3D(faces[i].vertices[2]->x, faces[i].vertices[2]->y, faces[i].vertices[2]->z);

            distp1p2 = distancePointToPoint(p1, p2);
            distp1p3 = distancePointToPoint(p1, p3);
            distp2p3 = distancePointToPoint(p2, p3);

            edgeMax = std::max(std::max(distp1p2, distp1p3), distp2p3);
            edgeMin = std::min(std::min(distp1p2, distp1p3), distp2p3);

            ratio += edgeMin / edgeMax;
        } else {
            std::cerr << "face sans 3 aretes" << std::endl;
        }
    }

    ratio = ratio / faces.size();
    return ratio;
}

bool isInVector (std::vector<Edge*>& edges, Edge* e) {

    for (unsigned int i = 0; i < edges.size(); ++i)
        if (edges[i] == e)
            return true;

    return false;
}

Edge* getNextEdge (std::vector<Edge*> edges, Edge* edge, std::vector<Edge*>& edgesTraited) {

     for (unsigned int i = 0; i < edges.size(); ++i) {
         if (((edge->v2 == edges[i]->v1 || (edge->v2 == edges[i]->v2)) ||  (edge->v1 == edges[i]->v1 || (edge->v1 == edges[i]->v2))) && !(edge->v1 == edges[i]->v1 && (edge->v2 == edges[i]->v2)) && !isInVector(edgesTraited, edge))
            return edges[i];
     }

     return NULL;
}

int Quality::nbHole(std::vector<Edge*>& edges) {

    int cmp;

    std::vector<Edge*> e;
    Edge* nextEdge;

    for (unsigned int i = 1; i < edges.size(); ++i) {

        if (!isInVector(e, edges[i])) {
            nextEdge = getNextEdge(edges, edges[i], e);

            while (nextEdge != NULL) {
                e.push_back(nextEdge);
                nextEdge = getNextEdge(edges, edges[i], e);
            }

            cmp++;
        }
    }

    std::cout << "trous = " << cmp << std::endl;
    return cmp;
}





void Quality::extractContours (std::vector<Edge *> &edges, std::vector<Edge *> &f, std::vector<Edge>& nonVariety) {

    for (unsigned int i = 0; i < edges.size(); ++i) {

        if (edges[i]->faces.size() < 2) {
            f.push_back(edges[i]);
            edges[i]->v1->r = 1.f;
            edges[i]->v1->g = edges[i]->v1->b = 0.f;
        }
        else if (edges[i]->faces.size() > 2) {
            nonVariety.push_back(*(edges[i]));
        }
    }

    std::cout << f.size() << " bords; " << nonVariety.size() << " non variété." << std::endl;
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
