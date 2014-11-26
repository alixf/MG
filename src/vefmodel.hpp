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
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;

    VEFModel();

    Edge* getOrAddEdge(EdgeMap& edgesMap, Vertex* v1, Vertex* v2, Face* face)
    {
        if(edgesMap.find(v1) != edgesMap.end() && edgesMap[v1].find(v2) != edgesMap[v1].end())
            return edgesMap[v1][v2];
        if(edgesMap.find(v2) != edgesMap.end() && edgesMap[v2].find(v1) != edgesMap[v2].end())
            return edgesMap[v2][v1];
        else
        {
            edges.push_back(Edge()); Edge& edge = edges.back();
            face->edges.push_back(&edge);
            v1->edges.push_back(&edge);
            v2->edges.push_back(&edge);
            edge.faces.push_back(face);
            edge.v1 = v1;
            edge.v2 = v2;

            if(edgesMap.find(v1) == edgesMap.end())
                edgesMap.insert(std::pair<Vertex*, std::map<Vertex*, Edge*> >(v1, std::map<Vertex*, Edge*>()));
            edgesMap[v1].insert(std::pair<Vertex*, Edge*>(v2, &edge));

            return &edge;
        }
//        Edge* alreadyExistingEdge = NULL;
//        for(std::vector<Edge>::iterator it = edges.begin(); it != edges.end() && alreadyExistingEdge == NULL; ++it)
//            if((it->v1 == edge.v1 && it->v2 == edge.v2) || (it->v1 == edge.v1 && it->v2 == edge.v2))
//                alreadyExistingEdge = &(*it);

//        if(alreadyExistingEdge == NULL)
//        {
//            edges.push_back(edge);
//            edges.back().v1 = edge.v1;
//            edges.back().v2 = edge.v2;
//            edges.back().v1->edges.push_back(&(edges.back()));
//            edges.back().v1->edges.push_back(&(edges.back()));
//            //edges.back().face1 = NULL;
//            //edges.back().face2 = NULL;
//            return &(edges.back());
//        }
//        return alreadyExistingEdge;
    }

    void load(const std::string& filename)
    {
        std::ifstream file(filename.c_str());
        if(!file.is_open())
            std::cerr << "Error in VEFModel::load : Cannot open file : " << filename << std::endl;

        std::istringstream linestream;

        std::map<Vertex*, std::map<Vertex*, Edge*> > edgesMap;

        edges.reserve(64);

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

                // Create vertex, Set coordinates of vertex
                vertices.push_back(Vertex());
                vertices.back().x = x;
                vertices.back().y = y;
                vertices.back().z = z;
            }
            else if(type == "f")
            {
                std::cout << "begin" << std::endl;
                // Retrieves vertices
                unsigned int vi1, vi2, vi3;
                linestream >> vi1 >> vi2 >> vi3;
                Vertex* v1 = &(vertices[vi1]);
                Vertex* v2 = &(vertices[vi2]);
                Vertex* v3 = &(vertices[vi3]);

                // Create face
                faces.push_back(Face()); Face& f = faces.back();
                // Add vertices to edge
                f.vertices.push_back(v1);
                f.vertices.push_back(v2);
                f.vertices.push_back(v3);

                // Create edges
                edges.push_back(Edge()); Edge& e1 = edges.back();
                edges.push_back(Edge()); Edge& e2 = edges.back();
                edges.push_back(Edge()); Edge& e3 = edges.back();
                // Add faces and vertices to edge
                e1.v1 = v1; e1.v2 = v2; e1.faces.push_back(&(faces.back()));
                e2.v1 = v2; e2.v2 = v3; e2.faces.push_back(&(faces.back()));
                e3.v1 = v3; e3.v2 = v1; e3.faces.push_back(&(faces.back()));

                // Add edges
                getOrAddEdge(edgesMap, v1, v2, &f);
                getOrAddEdge(edgesMap, v2, v3, &f);
                getOrAddEdge(edgesMap, v3, v1, &f);

                std::cout << "end" << std::endl;
            }

//            // Merge edges
//            for(std::vector<Edge*>::iterator it = edges.begin(); it != end(); ++it)
//            {
//                Edge& edge = *it;
//                if(edgesMap.find(edge.v1) != edgesMap.end() && edgesMap[edge.v1].find(edge.v2) != edgesMap.end())
//                {
//                    Edge* otherEdge = edgesMap[edge.v1][edge.v2];

//                    edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
//                }
//                else if(edgesMap.find(edge.v2) != edgesMap.end() && edgesMap[edge.v2].find(edge.v1) != edgesMap.end())
//                {
//                    Edge* otherEdge = edgesMap[edge.v2][edge.v1];
//                }
//                else
//                {
//                    if(edgesMap.find(edge.v1) == edgesMap.end())
//                        edgesMap.insert(std::pair<Vertex*, std::map<Vertex*, Edge*> >(edge.v1, std::map<Vertex*, Edge*>()));
//                    edgesMap[edge.v1].insert(std::pair<Vertex*, Edge*>(edge.v2, &edge));
//                }
//            }

//            for(std::vector<Face>::iterator it = faces.begin(); it != faces.end(); ++it)
//            {
//                Face& face = *it;

//                face.vertices.push_back();
//            }



                // Get or Add Edge, Set edge of vertices, Set vertices of edge
//                Edge e1; e1.v1 = &(vertices[vi1]); e1.v2 = &(vertices[vi2]); Edge* e1ptr; e1ptr = getOrAddEdge(e1);
//                Edge e2; e2.v1 = &(vertices[vi2]); e2.v2 = &(vertices[vi3]); Edge* e2ptr; e2ptr = getOrAddEdge(e2);
//                Edge e3; e3.v1 = &(vertices[vi3]); e3.v2 = &(vertices[vi1]); Edge* e3ptr; e3ptr = getOrAddEdge(e3);

                // Add Face, Set vertices of face, Set edges of face
                //faces.back().e1 = e1ptr;
                //faces.back().e2 = e2ptr;
                //faces.back().e3 = e3ptr;

                // Set faces of edge
                //e1ptr->faces.push_back(&(faces.back());
                //e2ptr->faces.push_back(&(faces.back());

                // Set face of vertices
//                vertices[vi1].faces.push_back(&(faces.back()));
//                vertices[vi2].faces.push_back(&(faces.back()));
//                vertices[vi3].faces.push_back(&(faces.back()));
//            }
        }

        file.close();
    }
};

#endif // VEFMODEL_HPP
