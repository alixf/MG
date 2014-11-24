#ifndef VEFMODEL_HPP
#define VEFMODEL_HPP

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>


struct Face;
struct Edge;
struct Face;

struct Vertex
{
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    std::vector<Edge*> edges;
    std::vector<Face*> faces;
};
typedef struct Vertex Vertex;

struct Edge
{
    Vertex* v1;
    Vertex* v2;
    Face* face1;
    Face* face2;
};
typedef struct Edge Edge;

struct Face
{
    Vertex* v1;
    Vertex* v2;
    Vertex* v3;
    Edge* e1;
    Edge* e2;
    Edge* e3;
};
typedef struct Face Face;

class VEFModel
{
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;

    VEFModel();

    Edge* getOrAddEdge(const Edge& edge)
    {
        Edge* alreadyExistingEdge = NULL;
        for(std::vector<Edge>::iterator it = edges.begin(); it != edges.end() && alreadyExistingEdge == NULL; ++it)
            if((it->v1 == edge.v1 && it->v2 == edge.v2) || (it->v1 == edge.v1 && it->v2 == edge.v2))
                alreadyExistingEdge = &(*it);

        if(alreadyExistingEdge == NULL)
        {
            edges.push_back(edge);
            edges.back().v1 = edge.v1;
            edges.back().v2 = edge.v2;
            edges.back().v1->edges.push_back(&(edges.back()));
            edges.back().v1->edges.push_back(&(edges.back()));
            edges.back().face1 = NULL;
            edges.back().face2 = NULL;
            return &(edges.back());
        }
        return alreadyExistingEdge;
    }

    void load(const std::string& filename)
    {
        std::ifstream file(filename.c_str());
        if(!file.is_open())
            std::cerr << "Error in VEFModel::load : Cannot open file : " << filename << std::endl;

        std::istringstream linestream;

        unsigned int i = 0;

        for(std::string line; getline(file, line);)
        {
            std::cout << i++ << std::endl;

            if(line[0] == '#' || line.size() <= 0)
                continue;

            linestream.str(line);
            linestream.seekg(0);

            // Retrieve type (v for vertex, f for face, vn for vertex normal)
            std::string type;
            linestream >> type;

            if(type == "v")
            {
                // Retriveve coordinates
                float x, y, z;
                linestream >> x >> y >> z;

                // Add vertex, Set coordinates of vertex
                vertices.push_back(Vertex());
                vertices.back().x = x;
                vertices.back().y = y;
                vertices.back().z = z;
            }
            else if(type == "f")
            {
                // Retrieves vertices
                unsigned int vi1, vi2, vi3;
                linestream >> vi1 >> vi2 >> vi3;

                // Get or Add Edge, Set edge of vertices, Set vertices of edge
                Edge e1; e1.v1 = &(vertices[vi1]); e1.v2 = &(vertices[vi2]); Edge* e1ptr; e1ptr = getOrAddEdge(e1);
                Edge e2; e2.v1 = &(vertices[vi2]); e2.v2 = &(vertices[vi3]); Edge* e2ptr; e2ptr = getOrAddEdge(e2);
                Edge e3; e3.v1 = &(vertices[vi3]); e3.v2 = &(vertices[vi1]); Edge* e3ptr; e3ptr = getOrAddEdge(e3);

                // Add Face, Set vertices of face, Set edges of face
                faces.push_back(Face());
                faces.back().v1 = &(vertices[vi1]);
                faces.back().v2 = &(vertices[vi2]);
                faces.back().v3 = &(vertices[vi3]);
                faces.back().e1 = e1ptr;
                faces.back().e2 = e2ptr;
                faces.back().e3 = e3ptr;

                // Set faces of edge
                if(e1ptr->face1 == NULL)
                    e1ptr->face1 = &(faces.back());
                else if(e1ptr->face2 == NULL)
                    e1ptr->face2 = &(faces.back());
                else
                    std::cerr << "Error : an edge corresponds to two faces." << std::endl;

                // Set face of vertices
                vertices[vi1].faces.push_back(&(faces.back()));
                vertices[vi2].faces.push_back(&(faces.back()));
                vertices[vi3].faces.push_back(&(faces.back()));
            }
        }
    }
};

#endif // VEFMODEL_HPP
