#ifndef QUALITY_H
#define QUALITY_H

#include <iostream>

class Quality
{
public :
    void contours (const double* edges, const unsigned int nbEdges, const double* faces, const unsigned int nbFaces, std::vector<double>& f, std::vector<double>& nonVariety);

private :

};

#endif // QUALITY_H
