#include <Eigen/Geometry>

#include "recalibration.hpp"

void Recalibration::translate (std::vector<float>& vertices, const QVector3D &p) {

    for (int i = 0; i < vertices.size(); i += 3) {
        vertices[i] += p[0];
        vertices[i+1] += p[1];
        vertices[i+2] += p[2];
    }
}

void Recalibration::rotate (GLfloat* vertices, const QVector3D &angle) {

    Eigen::AngleAxisf angleX (angle.x, QVector3D(1, 0, 0));
    Eigen::AngleAxisf angleY (angle.y, QVector3D(0, 1, 0));
    Eigen::AngleAxisf angleZ (angle.z, QVector3D(0, 0, 1));

    Eigen::Quaternionf q = Quaternionf(angle);

    for (int i = 0; i < vertices.size(); i += 3) {
        // TODO
    }
}

void Recalibration::recalibrate (std::vector<float>& vertices, const QVector3D &v1, const QVector3D &v2, const QVector3D &v3, const QVector3D &v1p, const QVector3D &v2p, const QVector3D &v3p) {

    // Translation
    QVector3D vecTranslation = vectorPointToPoint(v1, v1p);
    translate(vertices, vecTranslation);

    // Rotation
    float angle = angleBetweenVectors(v1p, v2, v1p, v2); // Attention v1 doit valoir v1p (ou l'inverse -> vérifier)
    rotate (vertices, angle);

    // Modifier modèle
}

QVector3D Recalibration::vectorPointToPoint(const QVector3D &p1,const  QVector3D &p2) const{

    return QVector3D(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
}


float Recalibration::angleBetweenVectors(const QVector3D& ov1, const QVector3D& fv1, const QVector3D& ov2, const QVector3D& fv2) const {

    QVector3D dirv1 = QVector3D(fv1.x - fv1.x, fv1.y - ov1.y, fv1.z - ov1.z);
    QVector3D dirv2 = QVector3D(fv2.x - fv2.x, fv2.y - ov2.y, fv2.z - ov2.z);

    float scalar = dirv1.x * dirv2.x + irv1.y * dirv2.y + irv1.z * dirv2.z;

    float norm = sqrt(dirv1.x() * dirv1.x() + dirv1.y() * dirv1.y() + dirv1.z() * dirv1.z()) *
                 sqrt(dirv2.x() * dirv2.x() + dirv2.y() * dirv2.y() + dirv2.z() * dirv2.z());

    return acos(scalarP / norm);
}
