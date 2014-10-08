<<<<<<< HEAD
#ifndef OCTREE_HPP
#define OCTREE_HPP
=======
#ifndef OCTREE_H
#define OCTREE_H
>>>>>>> fabb2a2e507b08d854e9a7e70507b01d9ab997bf

#include <fstream>
#include <iostream>
#include <sstream>
#include <QVector3D>
#include <QPoint>

/*class point3D {

private:
    float x, y,z;
};*/


<<<<<<< HEAD
class Octree
{
public :
    Octree(const std::vector<float>& vertices, const std::vector<int>& indexes, unsigned int maxHeight, unsigned int maxVertices, float halfSize, const QVector3D& center);
    std::vector<float> getNbOf(const QVector3D &p, float distance);
    QVector3D getFirstCollision(const QVector3D &v, float distance);
=======
class octree {

public :

    octree(std::vector<float> vertices, std::vector<int> indicesPoints, unsigned int height, unsigned int nbMax, float halfSize, QVector3D center);
    std::vector<float> getNbOf(const QVector3D &p, float distance);
    QVector3D getFirstCollision(const QVector3D &v, float distance);
    void printOctree();
>>>>>>> fabb2a2e507b08d854e9a7e70507b01d9ab997bf
    //iterator beginAt(unsigned int level);
    //iterator end();

private :
<<<<<<< HEAD
    std::vector<QVector3D> getCenters (QVector3D initCenter, float halfSize);
    int getChildIndex(QVector3D p);

    const std::vector<float>& m_vertices;
    std::vector<int> m_indexes;
    Octree* m_children[8];
    QVector3D m_center;
    float m_halfSize;
};

#endif // OCTREE_HPP
=======

    std::vector<QVector3D> getCenters (QVector3D initCenter, float halfSize);

    float halfSize;
    QVector3D center;
    std::vector<int> indicesPoints;
    octree *fils[8];
};

#endif // OCTREE_H
>>>>>>> fabb2a2e507b08d854e9a7e70507b01d9ab997bf
