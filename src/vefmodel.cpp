#include "vefmodel.hpp"

VEFModel::VEFModel()
{
}

std::vector<Vertex*>
VEFModel::getNeighBH(const Vertex* v)
{
    std::vector<Vertex*> NBH;

    std::vector<Edge*>::const_iterator it = v->edges.begin();
    std::vector<Edge*>::const_iterator end = v->edges.end();

    for(unsigned int i = 0; it != end; ++it, ++i)
    {
        if(v != (*it)->v1)
            NBH.push_back((*it)->v1);
        else
            NBH.push_back((*it)->v2);
    }
    return NBH;
}

// TODO : Use one of the method described in poly
void
VEFModel::computeNormal(Vertex* v)
{
    std::vector<Face*>::iterator it = v->faces.begin();
    std::vector<Face*>::const_iterator end = v->faces.end();

    struct float3{
        float x,y,z;
    };

    // Vertex normal
    float3 nV;

    for(; it != end; ++it){
        Vertex* v1 = (*it)->vertices[0];
        Vertex* v2 = (*it)->vertices[1];
        Vertex* v3 = (*it)->vertices[2];

        float3 U, V;
        U.x = v2->x - v1->x;
        U.y = v2->y - v1->y;
        U.z = v2->z - v1->z;

        V.x = v3->x - v1->x;
        V.y = v3->y - v1->y;
        V.z = v3->z - v1->z;

        (*it)->nx = (U.y * V.z) - (U.z * V.y);
        (*it)->ny = (U.z * V.x) - (U.x * V.z);
        (*it)->nz = (U.x * V.y) - (U.y * V.x);

        nV.x += (*it)->nx;
        nV.y += (*it)->ny;
        nV.z += (*it)->nz;
    }
    unsigned int nbFaces = v->faces.size();
    v->nx = nV.x / nbFaces;
    v->ny = nV.y / nbFaces;
    v->nz = nV.z / nbFaces;
}
