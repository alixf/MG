#ifndef RECALIBRATION_H
#define RECALIBRATION_H

#include <QVector3D>
#include <QPoint>

class Recalibration
{
public :


private :
    void translate (std::vector<float>& vertices, const QVector3D &p);
    void rotate (std::vector<float>& vertices, const QVector3D &angle);
    void recalibrate (std::vector<float>& vertices, const QVector3D &v1, const QVector3D &v2, const QVector3D &v3, const QVector3D &v1p, const QVector3D &v2p, const QVector3D &v3p);
    QVector3D vectorPointToPoint(const QVector3D &p1,const  QVector3D &p2) const;
    QVector3D angleBetweenVectors(const QVector3D& ov1, const QVector3D& fv1, const QVector3D& ov2, const QVector3D& fv2) const;
};

#endif // RECALIBRATION_H
