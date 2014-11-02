#ifndef RECALIBRATION_H
#define RECALIBRATION_H

class Recalibration
{
public :


private :
    void Recalibration::translate (std::vector<float>& vertices, const QVector3D &p);
    void rotate (GLfloat* vertices, const QVector3D &angle);
    QVector3F vectorPointToPoint(const QVector3D &p1,const  QVector3D &p2) const;
};

#endif // RECALIBRATION_H
