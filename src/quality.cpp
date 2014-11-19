#include "quality.h"

void contours (const double* edges, const unsigned int nbEdges, const double* faces, const unsigned int nbFaces, std::vector<double>& f, std::vector<double>& nonVariety) {

    int cmp;

    for (int i = 0; i < nbEdges; ++i) {
        cmp = 0;
        for (int j = 0; j < nbFaces; ++j) {
            if (edges[i] == *faces[j] || edges[i] == *faces[j+1] || edges[i] == *faces[j+2]) {
                cmp++;
            }
        }

        if (cmp < 2) {
            f.push_back(edges);
        }
        else if (cmp > 2) {
            nonVariety.push_back(edges);
        }
    }
}
