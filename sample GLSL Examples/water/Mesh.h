// Copyright Jay Conrod 2010.
// http://jayconrod.com
// jayconrod@gmail.com
//
// You are free to modify and distribute this code as long as this
// copyright notice is maintained.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

#ifndef Mesh_h
#define Mesh_h

#include <boost/shared_array.hpp>
#include <GL/gl.h>

class Vector;
struct Pair;

class Mesh {
    // Constructors
    public:
    Mesh(int vertexCount,
         boost::shared_array<Vector> vertices,
         boost::shared_array<Vector> normals,
         boost::shared_array<Pair> texCoords,
         int indexCount,
         boost::shared_array<int> indices);

    ~Mesh();

    // Methods
    public:
    void generateNormals();
    void generateVbos();
    bool hasVbos() const;

    int getNumVertices() const;
    boost::shared_array<Vector> getVertices();
    GLuint getVertexVbo() const;

    bool hasNormals() const;
    boost::shared_array<Vector> getNormals();
    GLuint getNormalVbo() const;

    bool hasTexCoords() const;
    boost::shared_array<Pair> getTexCoords();
    GLuint getTexCoordVbo() const;

    int getNumTriangles() const;
    boost::shared_array<int> getIndices();
    GLuint getIndexVbo() const;

    // Fields
    private:
    int vertexCount;
    boost::shared_array<Vector> vertices;
    GLuint vertexVbo;
    boost::shared_array<Vector> normals;
    GLuint normalVbo;
    boost::shared_array<Pair> texCoords;
    GLuint texCoordVbo;
    int indexCount;
    boost::shared_array<int> indices;
    GLuint indexVbo;
};

#endif
