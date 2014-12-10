#include "vefmodel.hpp"
#include "octree.hpp"
#include "math.h"
#include "quality.hpp"

void Quality::geometrie (std::vector<Face>& faces) {

    QVector3D p1;
    QVector3D p2;
    QVector3D p3;

    float distp1p2;
    float distp1p3;
    float distp2p3;

    float distBase;
    QVector3D vecBase;

    float ratio = 1;

    for (unsigned int i = 0; i < faces.size(); ++i) {
        if (faces[i].edges.size() == 3) {
            p1 = QVector3D(faces[i].vertices[0]->x, faces[i].vertices[0]->y, faces[i].vertices[0]->z);
            p2 = QVector3D(faces[i].vertices[1]->x, faces[i].vertices[1]->y, faces[i].vertices[1]->z);
            p3 = QVector3D(faces[i].vertices[2]->x, faces[i].vertices[2]->y, faces[i].vertices[2]->z);

            distp1p2 = distancePointToPoint(p1, p2);
            distp1p3 = distancePointToPoint(p1, p3);
            distp2p3 = distancePointToPoint(p2, p3);

            if (distp1p2 > distp1p3 && distp1p2 > distp2p3) {
                    distBase = distp1p2;
                    vecBase = vectorPointToPoint(p1, p2);
                    distPoint = distancePointToRay(p1, vecBase, p3);
            }
            else if (distp1p3 > distp1p2 && distp1p3 > distp2p3){
                    distBase = distp1p3;
                    vecBase = vectorPointToPoint(p1, p3);
                    distPoint = distancePointToRay(p3, vecBase, p2);
            }
            else {
                distBase = distp1p3;
                vecBase = vectorPointToPoint(p1, p3);
                distPoint = distancePointToRay(p3, vecBase, p2);
            }

	    ratio += (distPoint / distBase);
        }
    }

    ratio = ratio / faces.size();
}

void Quality::contours (std::vector<Edge>& edges, std::vector<Edge>& f, std::vector<Edge>& nonVariety) {

    for (unsigned int i = 0; i < edges.size(); ++i) {

        if (edges[i].faces.size() < 2) {
            f.push_back(edges[i]);
        }
        else if (edges[i].faces.size() > 2) {
            nonVariety.push_back(edges[i]);
        }
    }

    std::cout << f.size() << " bords; " << nonVariety.size() << "non variété." << std::endl;
}

float Quality::distancePointToPoint(QVector3D &pos1, QVector3D &pos2){
    return sqrt((pos1.x() - pos2.x()) * (pos1.x() - pos2.x()) + (pos1.y() - pos2.y()) * (pos1.y() - pos2.y()) + (pos1.z() - pos2.z()) * (pos1.z() - pos2.z()));

}

QVector3D Quality::vectorPointToPoint(QVector3D &p1, QVector3D &p2) {

    return QVector3D(p2.x() - p1.x(), p2.y() - p1.y(), p2.z() - p1.z());
}

float Quality::distancePointToRay(QVector3D& origin, QVector3D& dir, QVector3D& point) {
    QVector3D dirToPoint = QVector3D(point.x() - origin.x(), point.y() - origin.y(), point.z() - origin.z());

    float scalarP = dir.x() * dirToPoint.x() +  dir.y() * dirToPoint.y() + dir.z() * dirToPoint.z();
    float norm = sqrt(dir.x() * dir.x() + dir.y() * dir.y() + dir.z() * dir.z()) *
                 sqrt(dirToPoint.x() * dirToPoint.x() + dirToPoint.y() * dirToPoint.y() + dirToPoint.z() * dirToPoint.z());

    float angle =  acos(scalarP / norm);

    return sin(angle) * distancePointToPoint(origin, point);
}
