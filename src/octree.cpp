#include "octree.hpp"
#include <cmath>


Octree::Octree(std::vector<float>& vertices, const std::vector<int>& indexes, unsigned int maxHeight, unsigned int maxVertices, float halfSize, const QVector3D& center, const unsigned int maxLeaf) : m_vertices(vertices), m_center(center), m_halfSize(halfSize)
{

    std::cout << "new octree at " << m_center.x() << ";" << m_center.y() << ";" << m_center.z() << std::endl;
    // Check if vertices are inside the octree
    for (unsigned int  i = 0; i < indexes.size(); ++i)
    {
        if (vertices[indexes[i]*6+0] < m_center.x() + m_halfSize &&
            vertices[indexes[i]*6+0] > m_center.x() - m_halfSize &&
            vertices[indexes[i]*6+1] < m_center.y() + m_halfSize &&
            vertices[indexes[i]*6+1] > m_center.y() - m_halfSize &&
            vertices[indexes[i]*6+2] < m_center.z() + m_halfSize &&
            vertices[indexes[i]*6+2] > m_center.z() - m_halfSize)
                m_indexes.push_back(indexes[i]);
    }
    if(m_indexes.size() > 0) ++m_nbLeaf;
    if(vertices.size() == indexes.size()) m_nbLeaf = 0;

    for(int i = 0; i < 8; i++)
        m_children[i] = NULL;

    // Create children if needed

    if ((m_indexes.size() > maxVertices && maxHeight > 0) && m_nbLeaf < maxLeaf)
    {

        std::vector<QVector3D> childCenters = getCenters(center, halfSize / 2);

        for (unsigned int i = 0; i < childCenters.size(); ++i)
            m_children[i] = new Octree(vertices, m_indexes, maxHeight-1, maxVertices, halfSize/2, childCenters[i], maxLeaf);
    }

    if(maxLeaf > 0){
        std::vector<int> index;
        index.resize(maxLeaf);
        std::vector<float> vertice;
        vertice.resize(maxLeaf*3);

    }
}

std::vector<QVector3D> Octree::getCenters (QVector3D center, float halfSize)
{
    std::vector<QVector3D> result;
    result.resize(8);
    result[0] = QVector3D(center.x() - halfSize, center.y() - halfSize, center.z() - halfSize);
    result[1] = QVector3D(center.x() - halfSize, center.y() - halfSize, center.z() + halfSize);
    result[2] = QVector3D(center.x() - halfSize, center.y() + halfSize, center.z() - halfSize);
    result[3] = QVector3D(center.x() - halfSize, center.y() + halfSize, center.z() + halfSize);
    result[4] = QVector3D(center.x() + halfSize, center.y() - halfSize, center.z() - halfSize);
    result[5] = QVector3D(center.x() + halfSize, center.y() - halfSize, center.z() + halfSize);
    result[6] = QVector3D(center.x() + halfSize, center.y() + halfSize, center.z() - halfSize);
    result[7] = QVector3D(center.x() + halfSize, center.y() + halfSize, center.z() + halfSize);
    return result;
}

std::vector<float> Octree::getNbOf(const QVector3D &p, float distance) const
{
    if(m_children[0] == NULL)
    {
        std::vector<float> result;
        for(std::vector<int>::const_iterator it = m_indexes.begin(); it != m_indexes.end(); ++it)
        {
            float x = m_vertices[(*it)*6+0] - m_center.x();
            float y = m_vertices[(*it)*6+1] - m_center.y();
            float z = m_vertices[(*it)*6+2] - m_center.z();
            if(sqrt(x*x+y*y+z*z) < distance)
            {
                result.push_back(m_vertices[(*it)*6+0]);
                result.push_back(m_vertices[(*it)*6+1]);
                result.push_back(m_vertices[(*it)*6+2]);
                result.push_back(m_vertices[(*it)*6+3]);
                result.push_back(m_vertices[(*it)*6+4]);
                result.push_back(m_vertices[(*it)*6+5]);
            }
        }
        return result;
    }
    return m_children[getChildIndex(p)]->getNbOf(p, distance);
}

QVector3D Octree::getFirstCollision(std::vector<float> vertices, const QVector3D &pos, const QVector3D &dir, float width) const{

    if (m_children != NULL) {

        if (pos.x() < m_center.x() && pos.y() < m_center.y() && pos.z() < m_center.z())
            m_children[0]->getFirstCollision(vertices, pos, dir, width);
        else if (pos.x() < m_center.x() && pos.y() < m_center.y() && pos.z() > m_center.z())
            m_children[1]->getFirstCollision(vertices, pos, dir, width);
        else if (pos.x() < m_center.x() && pos.y() > m_center.y() && pos.z() < m_center.z())
            m_children[2]->getFirstCollision(vertices, pos, dir, width);
        else if (pos.x() < m_center.x() && pos.y() > m_center.y() && pos.z() > m_center.z())
            m_children[3]->getFirstCollision(vertices, pos, dir, width);
        else if (pos.x() > m_center.x() && pos.y() < m_center.y() && pos.z() < m_center.z())
            m_children[4]->getFirstCollision(vertices, pos, dir, width);
        else if (pos.x() > m_center.x() && pos.y() < m_center.y() && pos.z() > m_center.z())
            m_children[5]->getFirstCollision(vertices, pos, dir, width);
        else if (pos.x() > m_center.x() && pos.y() > m_center.y() && pos.z() < m_center.z())
            m_children[6]->getFirstCollision(vertices, pos, dir, width);
        else
            m_children[7]->getFirstCollision(vertices, pos, dir, width);
    }

    else {
        std::vector<int> pointsInter;

        for (size_t i = 0; i < m_indexes.size(); ++i) {
            if (distancePointToRay(pos, dir, QVector3D(vertices[m_indexes[i]*3], vertices[m_indexes[i]*3 +1], vertices[m_indexes[i]*3 + 2]) ) < width)
                pointsInter.push_back(i);
        }

        float min = pointsInter[0];
        QVector3D pointDistMin;

        for (size_t i = 1; i < pointsInter.size(); ++i) {
            const QVector3D vec = QVector3D(vertices[pointsInter[i]*3], vertices[pointsInter[i]*3+1], vertices[pointsInter[i]*3+2]);
            float distance = distancePointToPoint(vec, pos);
            if (distance < min) {
                min = distance;
                pointDistMin = QVector3D(vertices[pointsInter[i]], vertices[pointsInter[i]+ 1], vertices[pointsInter[i]+2]);
            }
        }

        return pointDistMin;

    }

    return QVector3D(0,0,0);
}

float Octree::distancePointToRay(const QVector3D& origin, const QVector3D& dir, const QVector3D& point) const
{
    QVector3D dirToPoint = QVector3D(point.x() - origin.x(), point.y() - origin.y(), point.z() - origin.z());

    float scalarP = dir.x() * dirToPoint.x() +  dir.y() * dirToPoint.y() + dir.z() * dirToPoint.z();
    float norm = sqrt(dir.x() * dir.x() + dir.y() * dir.y() + dir.z() * dir.z()) *
                 sqrt(dirToPoint.x() * dirToPoint.x() + dirToPoint.y() * dirToPoint.y() + dirToPoint.z() * dirToPoint.z());

    float angle =  acos(scalarP / norm);

    return sin(angle) * distancePointToPoint(origin, point);


}

float Octree::distancePointToPoint(const QVector3D &pos1,const  QVector3D &pos2) const{
    return sqrt((pos1.x() - pos2.x()) * (pos1.x() - pos2.x()) + (pos1.y() - pos2.y()) * (pos1.y() - pos2.y()) + (pos1.z() - pos2.z()) * (pos1.z() - pos2.z()));

}

int Octree::getChildIndex(QVector3D p) const
{
    int index = 0;
    for(int x = -1; x <= 1; x += 2)
        for(int y = -1; y <= 1; y += 2)
            for(int z = -1; z <= 1; z += 2)
            {
                if(x * p.x() < this->m_center.x() && y * p.y() < this->m_center.y() && z * p.z() < this->m_center.z())
                   return index;
                ++index;
            }

    return index;
}
/*iterator octree::beginAt(unsigned int level) {

}

iterator octree::end() {

}
*/

void
Octree::printOctree() const
{

}

void
Octree::decimation(std::vector<int>& indexes, std::vector<float>& vertices, unsigned int currentIndex)
{    
    //is a leaf
    if(m_children == NULL){
        float meanX, meanY, meanZ;
        meanX = meanY = meanZ = 0.f;
        std::size_t nbIndex =  m_indexes.size();
        for(std::size_t i = 0; i < nbIndex; ++i){
            meanX += m_vertices[m_indexes[i]];
            meanY += m_vertices[m_indexes[i]+1];
            meanZ += m_vertices[m_indexes[i]+2];
        }
        meanX /= nbIndex;
        meanY /= nbIndex;
        meanZ /= nbIndex;
        vertices.push_back(meanX);
        vertices.push_back(meanY);
        vertices.push_back(meanZ);
        indexes.push_back(currentIndex++);
    } else {
        for(int i = 0; i < 8; ++i){
            m_children[i]->decimation(indexes, vertices, ++currentIndex);
        }
    }
}
