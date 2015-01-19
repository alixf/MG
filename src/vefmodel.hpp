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
    float x,y,z;
    float nx;
    float ny;
    float nz;
    float r,g,b;
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
    float nx, ny, nz;
};
typedef struct Face Face;

typedef std::map<Vertex*, std::map<Vertex*, Edge*> > EdgeMap;

class VEFModel
{
private:
public:
    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;
    std::vector<Face*> faces;
    VEFModel();


    inline Edge* getOrAddEdge(EdgeMap& edgesMap, Vertex* v1, Vertex* v2)
    {
        if(edgesMap.find(v1) != edgesMap.end() && edgesMap[v1].find(v2) != edgesMap[v1].end()){
            return edgesMap[v1][v2];
        }
        if(edgesMap.find(v2) != edgesMap.end() && edgesMap[v2].find(v1) != edgesMap[v2].end()){
            return edgesMap[v2][v1];
        }

        Edge* e = new Edge;
        e->v1 = v1;
        e->v2 = v2;
        e->faces.reserve(2);
        edges.push_back(e);
        v1->edges.push_back(edges.back());
        v2->edges.push_back(edges.back());


        if(edgesMap.find(v1) == edgesMap.end())
            edgesMap.insert(std::pair<Vertex*, std::map<Vertex*, Edge*> >(v1, std::map<Vertex*, Edge*>()));
        edgesMap[v1].insert(std::pair<Vertex*, Edge*>(v2, edges.back()));

        return edges.back();

    }

    void load(const std::string& filename)
    {
        std::ifstream file(filename.c_str());
        if(!file.is_open())
            std::cerr << "Error in VEFModel::load : Cannot open file : " << filename << std::endl;

        std::istringstream linestream;

        std::map<Vertex*, std::map<Vertex*, Edge*> > edgesMap;

        //edges.reserve(16384);
        for(std::string line; getline(file, line);)
        {
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
                Vertex* v = new Vertex;
                v->x = x; v->y = y; v->z = z;
                v->r = v->g = v->b = 0.2f;
                vertices.push_back(v);
            }
            else if(type == "f")
            {
                // Retrieves vertices
                std::string part1,part2,part3;
                unsigned int vi1 = 0, vi2 = 0, vi3 = 0;

                linestream >> part1 >> part2 >> part3;

                linestream.str(part1);
                linestream.seekg(0);
                linestream >> vi1;

                linestream.str(part2);
                linestream.seekg(0);
                linestream >> vi2;

                linestream.str(part3);
                linestream.seekg(0);
                linestream >> vi3;

                Vertex* v1 = vertices[vi1-1];
                Vertex* v2 = vertices[vi2-1];
                Vertex* v3 = vertices[vi3-1];
                // Create face
                Face* f = new Face;
                faces.push_back(f);
                // Add vertices to edge
                f->vertices.push_back(v1);
                f->vertices.push_back(v2);
                f->vertices.push_back(v3);
                v1->faces.push_back(f);
                v2->faces.push_back(f);
                v1->faces.push_back(f);
                // Add edges
                Edge *e1,*e2,*e3;

                e1 = getOrAddEdge(edgesMap, v1, v2);
                e2 = getOrAddEdge(edgesMap, v2, v3);
                e3 = getOrAddEdge(edgesMap, v3, v1);
                f->edges.push_back(e1);
                f->edges.push_back(e2);
                f->edges.push_back(e3);
                e1->faces.push_back(f);
                e2->faces.push_back(f);
                e3->faces.push_back(f);
            }
        }

        file.close();
    }

    std::vector<Vertex *> getNeighBH(const Vertex* v);
    void computeNormal(Vertex *v);
};

#endif // VEFMODEL_HPP
