#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <QVector3D>
#include <QPoint>

class Octree
{
public :
    Octree(const std::vector<float>& vertices, const std::vector<int>& indexes, unsigned int maxHeight, unsigned int maxVertices, float halfSize, const QVector3D& center);
    std::vector<float> getNbOf(const QVector3D &p, float distance);
    QVector3D getFirstCollision(std::vector<float> vertices, const QVector3D &pos, const QVector3D &dir, float width);
    void printOctree();
    //iterator beginAt(unsigned int level);
    //iterator end();

private :
    float distancePointToRay(const QVector3D& origin, const QVector3D& dir, const QVector3D& point);
    float distancePointToPoint(const QVector3D &pos1, const QVector3D &pos2);
    std::vector<QVector3D> getCenters (QVector3D initCenter, float halfSize);
    int getChildIndex(QVector3D p);

    const std::vector<float>& m_vertices;
    std::vector<int> m_indexes;
    Octree* m_children[8];
    QVector3D m_center;
    float m_halfSize;
};

#endif // OCTREE_HPP
