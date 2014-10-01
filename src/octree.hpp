#ifndef OCTREE_H
#define OCTREE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <QVector3D>
#include <QPoint>

/*class point3D {

private:
    float x, y,z;
};*/


class octree {

public :

    octree(std::vector<float> vertices, std::vector<int> indicesPoints, unsigned int height, unsigned int nbMax, float halfSize, QVector3D center);
    std::vector<float> getNbOf(const QVector3D &p, float distance);
    QVector3D getFirstCollision(const QVector3D &v, float distance);
    void printOctree();
    //iterator beginAt(unsigned int level);
    //iterator end();

private :

    std::vector<QVector3D> getCenters (QVector3D initCenter, float halfSize);

    float halfSize;
    QVector3D center;
    std::vector<int> indicesPoints;
    octree *fils[8];
};

#endif // OCTREE_H
