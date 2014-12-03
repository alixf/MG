#ifndef VEFMODEL_HPP
#define VEFMODEL_HPP

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>


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
    std::vector<Face*> faces;
};
typedef struct Edge Edge;

struct Face
{
    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;
};
typedef struct Face Face;

typedef std::map<Vertex*, std::map<Vertex*, Edge*> > EdgeMap;

class VEFModel
{
private:
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;
    VEFModel();


    inline Edge* getOrAddEdge(EdgeMap& edgesMap, Vertex* v1, Vertex* v2, Face* face)
    {
        if(edgesMap.find(v1) != edgesMap.end() && edgesMap[v1].find(v2) != edgesMap[v1].end())
            return edgesMap[v1][v2];
        if(edgesMap.find(v2) != edgesMap.end() && edgesMap[v2].find(v1) != edgesMap[v2].end())
            return edgesMap[v2][v1];




        Edge e;
        e.v1 = v1;
        e.v2 = v2;
        e.faces.push_back(face);
        edges.push_back(e);
        face->edges.push_back(&e);
        v1->edges.push_back(&(edges.back()));
        v2->edges.push_back(&(edges.back()));

        if(edgesMap.find(v1) == edgesMap.end())
            edgesMap.insert(std::pair<Vertex*, std::map<Vertex*, Edge*> >(v1, std::map<Vertex*, Edge*>()));
        //edgesMap[v1].insert(std::pair<Vertex*, Edge*>(v2, &edge));
        edgesMap[v1].insert(std::pair<Vertex*, Edge*>(v2, &(edges.back())));

        return &(edges.back());

    }

    void load(const std::string& filename)
    {
        std::ifstream file(filename.c_str());
        if(!file.is_open())
            std::cerr << "Error in VEFModel::load : Cannot open file : " << filename << std::endl;

        std::istringstream linestream;

        std::map<Vertex*, std::map<Vertex*, Edge*> > edgesMap;

        edges.reserve(16384);
        unsigned int i = 0;

        for(std::string line; getline(file, line);)
        {
            //std::cout << i++ << std::endl;

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
                Vertex* v1 = &(vertices[vi1-1]);
                Vertex* v2 = &(vertices[vi2-1]);
                Vertex* v3 = &(vertices[vi3-1]);
                // Create face
                faces.push_back(Face()); Face& f = faces.back();
                // Add vertices to edge
                f.vertices.push_back(v1);
                f.vertices.push_back(v2);
                f.vertices.push_back(v3);

                // Add edges
                f.edges.push_back(getOrAddEdge(edgesMap, v1, v2, &f));
                f.edges.push_back(getOrAddEdge(edgesMap, v2, v3, &f));
                f.edges.push_back(getOrAddEdge(edgesMap, v3, v1, &f));
            }
        }

        file.close();
    }
};

#endif // VEFMODEL_HPP
