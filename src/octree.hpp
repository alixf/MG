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
    Octree(std::vector<float>& vertices, const std::vector<int>& indexes, unsigned int maxHeight, unsigned int maxVertices, float halfSize, const QVector3D& center, const unsigned int maxLeaf = 0);
    std::vector<float> getNbOf(const QVector3D &p, float distance) const;
    QVector3D getFirstCollision(std::vector<float> vertices, const QVector3D &pos, const QVector3D &dir, float width) const;
    void printOctree() const;
    void decimation(std::vector<int>& indexes, std::vector<float>& vertices, unsigned int);
    static unsigned int getNbLeaf() {return Octree::m_nbLeaf;}
    //iterator beginAt(unsigned int level);
    //iterator end();


private :
    float distancePointToRay(const QVector3D& origin, const QVector3D& dir, const QVector3D& point) const;
    float distancePointToPoint(const QVector3D &pos1, const QVector3D &pos2) const;
    std::vector<QVector3D> getCenters (QVector3D initCenter, float halfSize);
    int getChildIndex(QVector3D p) const;

    std::vector<float>& m_vertices;
    std::vector<int> m_indexes;
    Octree* m_children[8];
    QVector3D m_center;
    float m_halfSize;

    static short INIT;
    static unsigned int m_nbLeaf;
};

#endif // OCTREE_HPP
