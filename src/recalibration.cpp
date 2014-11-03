#include <Eigen/Geometry>

#include "recalibration.h"

void Recalibration::translate (std::vector<float>& vertices, const QVector3D &p) {

    for (int i = 0; i < vertices.size(); i += 3) {
        vertices[i] += p[0];
        vertices[i+1] += p[1];
        vertices[i+2] += p[2];
    }
}

void Recalibration::rotate (std::vector<float>& vertices, const QVector3D &angle) {

    Eigen::Vector4f vec;
    Eigen::Vector4f vecRot;

    Eigen::Quaternionf q = Eigen::Quaternionf(0, angle.x(), angle.y(), angle.z());
    Eigen::Quaternionf qInv = q.inverse();

    for (int i = 0; i < vertices.size(); i += 3) {

        vec = Eigen::Vector4f(vertices[i], vertices[i+1], vertices[i+2], 0);

        vecRot = q * vec * qInv;

        vertices[i] = vecRot.x();
        vertices[i+1] = vecRot.y();
        vertices[i+2] = vecRot.z();
    }
}

void Recalibration::recalibrate (std::vector<float>& vertices, const QVector3D &v1, const QVector3D &v2, const QVector3D &v3, const QVector3D &v1p, const QVector3D &v2p, const QVector3D &v3p) {

    // Translation
    QVector3D vecTranslation = vectorPointToPoint(v1, v1p);
    translate(vertices, vecTranslation);

    // Rotation
    QVector3D angle1 = angleBetweenVectors(v1p, v2, v1p, v2); // A vérifier
    QVector3D angle2 = angleBetweenVectors(v1p, v3, v1p, v3); // A vérifier

    rotate (vertices, angle1 + angle2);
}

QVector3D Recalibration::vectorPointToPoint(const QVector3D &p1,const  QVector3D &p2) const{

    return QVector3D(p2.x() - p1.x(), p2.y() - p1.y(), p2.z() - p1.z());
}


QVector3D Recalibration::angleBetweenVectors(const QVector3D& ov1, const QVector3D& fv1, const QVector3D& ov2, const QVector3D& fv2) const {

    QVector3D res;

    // Axe X

    QVector3D dirv1 = QVector3D(fv1.x() - fv1.x(), 0, 0);
    QVector3D dirv2 = QVector3D(fv2.x() - fv2.x(), 0, 0);

    float scalar = dirv1.x() * dirv2.x();

    float norm = sqrt(dirv1.x() * dirv1.x()) * sqrt(dirv2.x() * dirv2.x());

    res.setX(acos(scalar / norm));

    // Axe Y

    dirv1 = QVector3D(0, fv1.y() - fv1.y(), 0);
    dirv2 = QVector3D(0, fv2.y() - fv2.y(), 0);

    scalar = dirv1.y() * dirv2.y();

    norm = sqrt(dirv1.y() * dirv1.y()) * sqrt(dirv2.y() * dirv2.y());

    res.setY(acos(scalar / norm));

    // Axe Z

    dirv1 = QVector3D(fv1.z() - fv1.z(), 0, 0);
    dirv2 = QVector3D(fv2.z() - fv2.z(), 0, 0);

    scalar = dirv1.z() * dirv2.z();

    norm = sqrt(dirv1.z() * dirv1.z()) * sqrt(dirv2.z() * dirv2.z());

    res.setZ(acos(scalar / norm));

    return res;

    /*

    QVector3D dirv1 = QVector3D(fv1.x - fv1.x, fv1.y - ov1.y, fv1.z - ov1.z);
    QVector3D dirv2 = QVector3D(fv2.x - fv2.x, fv2.y - ov2.y, fv2.z - ov2.z);

    float scalar = dirv1.x * dirv2.x + irv1.y * dirv2.y + irv1.z * dirv2.z;

    float norm = sqrt(dirv1.x() * dirv1.x() + dirv1.y() * dirv1.y() + dirv1.z() * dirv1.z()) *
                 sqrt(dirv2.x() * dirv2.x() + dirv2.y() * dirv2.y() + dirv2.z() * dirv2.z());

    return acos(scalarP / norm);*/
}
