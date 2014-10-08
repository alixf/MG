#include "octree.hpp"
#include <cmath>


/*pour i de 0 à nombreDeFace faire
   si face[i] est inclu dans la boite courante
      inclure i dans la liste des faces contenus dans le noeud
      incrémenté nombre de face contenu
   fsi
fpour

si nombre de faces contenu > valeur
   créer 8 fils pour le noeud courant
   recommencer avec chacun des 8 fils
fsi
*/

Octree::Octree(const std::vector<float>& vertices, const std::vector<int>& indexes, unsigned int maxHeight, unsigned int maxVertices, float halfSize, const QVector3D& center) : m_vertices(vertices), m_center(center), m_halfSize(halfSize)
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

    for(int i = 0; i < 8; i++)
        m_children[i] = NULL;

    // Create children if needed
    if (m_indexes.size() > maxVertices && maxHeight > 0)
    {
        std::vector<QVector3D> childCenters = getCenters(center, halfSize / 2);

        for (unsigned int i = 0; i < childCenters.size(); ++i)
            m_children[i] = new Octree(vertices, m_indexes, maxHeight-1, maxVertices, halfSize/2, childCenters[i]);
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

std::vector<float> Octree::getNbOf(const QVector3D &p, float distance)
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

QVector3D Octree::getFirstCollision(const QVector3D& /*v*/, float /*distance*/) {
    return QVector3D();
}

int Octree::getChildIndex(QVector3D p)
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
