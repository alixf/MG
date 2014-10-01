#include "octree.hpp"


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

octree::octree(std::vector<float> vertices, std::vector<int> indP, unsigned int height, unsigned int nbMax, float halfSize, QVector3D center) {

    this->center = center;
    this->halfSize = halfSize;

    // calcul de ses indices
    for (int  i = 0; i < indP.size(); ++i) {
        // Si vertices[indP[i]] est inclu
        if (vertices[indP[i]] > center.x() + halfSize ||
            vertices[indP[i]] < center.x() - halfSize ||
            vertices[indP[i]+1] > center.y() + halfSize ||
            vertices[indP[i]+1] < center.y() - halfSize ||
            vertices[indP[i]+2] > center.z() + halfSize ||
            vertices[indP[i]+2] < center.z() - halfSize)
            continue;
        indicesPoints.push_back(indP[i]);
    }



    if (indicesPoints.size() > nbMax && height > 0) {

        std::vector<QVector3D> tabCenter = getCenters(center, halfSize / 2);

        for (int i = 0; i < 8; ++i)
            fils[i] = new octree(vertices, indicesPoints, height - 1, nbMax, halfSize / 2, tabCenter[i]);
    }
}

std::vector<QVector3D> octree::getCenters (QVector3D initCenter, float halfSize) {
    std::vector<QVector3D> center;

    center[0] = QVector3D(initCenter.x() - halfSize, initCenter.y() - halfSize, initCenter.z() - halfSize);
    center[1] = QVector3D(initCenter.x() - halfSize, initCenter.y() - halfSize, initCenter.z() + halfSize);
    center[2] = QVector3D(initCenter.x() - halfSize, initCenter.y() + halfSize, initCenter.z() - halfSize);
    center[3] = QVector3D(initCenter.x() - halfSize, initCenter.y() + halfSize, initCenter.z() + halfSize);
    center[4] = QVector3D(initCenter.x() + halfSize, initCenter.y() - halfSize, initCenter.z() - halfSize);
    center[5] = QVector3D(initCenter.x() + halfSize, initCenter.y() - halfSize, initCenter.z() + halfSize);
    center[6] = QVector3D(initCenter.x() + halfSize, initCenter.y() + halfSize, initCenter.z() - halfSize);
    center[7] = QVector3D(initCenter.x() + halfSize, initCenter.y() + halfSize, initCenter.z() + halfSize);

    return center;
}

std::vector<float> octree::getNbOf(const QVector3D &p, float distance) {

}

QVector3D octree::getFirstCollision(const QVector3D &v, float distance) {

}

/*iterator octree::beginAt(unsigned int level) {

}

iterator octree::end() {

}
*/

void
octree::printOctree()
{

}
