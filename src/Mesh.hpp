#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <strstream>

#include "MathUtil.hpp"

namespace cgel 
{

    struct Vertex
    {
        Vec4f position;
        Vec3f textureCoordinate;
        Vec4f normal;
    };

    struct VertexRef
    {
        int32_t v, vt, vn;
    };

    struct Triangle
    {
        Vertex vertex0, vertex1, vertex2;
        Vec4f faceNormal;
        char asciiChar;
    };

    class Mesh
    {
        private:
            std::vector<Triangle> m_triangle_collection;

        public:
            void addTriangle(const Triangle &tri) 
            {
                m_triangle_collection.push_back(tri);
            }

            std::vector<Triangle> &getTriangleCollection() 
            {
                return m_triangle_collection;
            }
    };

    // https://stackoverflow.com/questions/52824956/how-can-i-parse-a-simple-obj-file-into-triangles
    Mesh constructMeshFromObjectFile(const std::string &fileName)
    {
        std::ifstream objectFile(fileName);
        if (!objectFile.is_open())
            throw std::runtime_error(fileName + "not found.");

        std::vector<Vec4f> vertexPositionCollection;
        std::vector<Vec3f> vertexTextureCoordinateCollection;
        std::vector<Vec4f> vertexNormalCollection;
        std::vector<Vertex> vertexCollection;
        Mesh mesh;

        std::string lineString;
        while(std::getline(objectFile, lineString))
        {
            std::istringstream lineStream(lineString);
            std::string lineType;
            lineStream >> lineType;

            if (lineType == "v")
            {
                float x = 0;
                float y = 0;
                float z = 0;
                float w = 1;
                lineStream >> x >> y >> z >> w;
                vertexPositionCollection.push_back(Vec4f{x, y, z, w});
            }

            else if (lineType == "vt")
            {
                float u = 0;
                float v = 0;
                float w = 0;
                lineStream >> u >> v >> w;
                vertexTextureCoordinateCollection.push_back(Vec3f{u, v, w});
            }

            else if (lineType == "vn")
            {
                float i = 0; 
                float j = 0; 
                float k = 0;
                lineStream >> i >> j >> k;
                vertexNormalCollection.push_back(Vec4f{i, j, k, 1}.unitH());
            }

            if( lineType == "f" )
            {
                std::vector<VertexRef> vertexRefCollection;
                std::string refString;
                while(lineStream >> refString)
                {
                    std::istringstream ref(refString);
                    std::string vString, vtString, vnString;

                    std::getline( ref, vString, '/' );
                    std::getline( ref, vtString, '/' );
                    std::getline( ref, vnString, '/' );

                    int v  = atoi( vString.c_str());
                    int vt = atoi(vtString.c_str());
                    int vn = atoi(vnString.c_str());

                    v  = ( v >= 0 ?  v : vertexPositionCollection.size() + v);
                    vt = (vt >= 0 ? vt : vertexTextureCoordinateCollection.size() + vt);
                    vn = (vn >= 0 ? vn : vertexNormalCollection.size() + vn );
                    vertexRefCollection.push_back( VertexRef{ v - 1, vt - 1, vn - 1 } );
                }

                for( size_t i = 1; i+1 < vertexRefCollection.size(); ++i )
                {
                    const VertexRef* p[3] = { &vertexRefCollection[0], &vertexRefCollection[i], &vertexRefCollection[i+1] };

                    Triangle triangle;
                    triangle.vertex0.position.assign(vertexPositionCollection[ p[0]->v ]);
                    triangle.vertex1.position.assign(vertexPositionCollection[ p[1]->v ]);
                    triangle.vertex2.position.assign(vertexPositionCollection[ p[2]->v ]);

                    triangle.vertex0.textureCoordinate.assign(vertexTextureCoordinateCollection[ p[0]->vt ]);
                    triangle.vertex1.textureCoordinate.assign(vertexTextureCoordinateCollection[ p[1]->vt ]);
                    triangle.vertex2.textureCoordinate.assign(vertexTextureCoordinateCollection[ p[2]->vt ]);

                    triangle.vertex0.normal.assign(vertexNormalCollection[ p[0]->vn ]);
                    triangle.vertex1.normal.assign(vertexNormalCollection[ p[1]->vn ]);
                    triangle.vertex2.normal.assign(vertexNormalCollection[ p[2]->vn ]);

                    Vec4f U(vertexPositionCollection[ p[1]->v ].subtractH(vertexPositionCollection[ p[0]->v ]));
                    Vec4f V(vertexPositionCollection[ p[2]->v ].subtractH(vertexPositionCollection[ p[0]->v ]));
                    triangle.faceNormal.assign(U.crossH(V).unitH());
                    triangle.asciiChar = ' ';

                    mesh.addTriangle(triangle);
                }
            }
        }
        objectFile.close();
        return mesh;
    }
}


#endif
