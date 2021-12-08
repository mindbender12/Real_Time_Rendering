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

#include "Mesh.h"

#include <vector>
#include <cassert>
#include <GL/gl.h>
#include "Utilities.h"
#include "Vector.h"

using namespace std;
using namespace boost;

Mesh::Mesh(int vertexCount,
           shared_array<Vector> vertices,
           shared_array<Vector> normals,
           shared_array<Pair> texCoords,
           int indexCount,
           shared_array<int> indices) :
    vertexCount(vertexCount),
    vertices(vertices),
    vertexVbo(0),
    normals(normals),
    normalVbo(0),
    texCoords(texCoords),
    texCoordVbo(0),
    indexCount(indexCount),
    indices(indices),
    indexVbo(0)
{
    assert(indexCount % 3 == 0);
}

Mesh::~Mesh() {
    if (vertexVbo)
        glDeleteBuffers(1, &vertexVbo);
    if (normalVbo)
        glDeleteBuffers(1, &normalVbo);
    if (texCoordVbo)
        glDeleteBuffers(1, &texCoordVbo);
    if (indexVbo)
        glDeleteBuffers(1, &indexVbo);
}

void Mesh::generateNormals() {
    normals.reset(new Vector[vertexCount]);
    for (int i = 0; i < indexCount; i += 3) {
        int tri[3] = { indices[i], indices[i + 1], indices[i + 2] };
        Vector a = vertices[tri[0]], 
            b = vertices[tri[1]],
            c = vertices[tri[2]];
        Vector ab = b - a, ac = c - a;
        Vector n = ab.cross(ac).normalize();
        for (int j = 0; j < 3; ++j)
            normals[tri[j]] = normals[tri[j]] + n;
    }

    for (int i = 0; i < vertexCount; ++i)
        normals[i] = normals[i].normalize();
}

void Mesh::generateVbos() {
    glGenBuffers(1, &vertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 vertexCount * sizeof(float) * 3, 
                 (GLfloat*) vertices.get(), 
                 GL_STATIC_DRAW);
    
    if (hasNormals()) {
        glGenBuffers(1, &normalVbo);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glBufferData(GL_ARRAY_BUFFER,
                     vertexCount * sizeof(float) * 3,
                     (GLfloat*) normals.get(),
                     GL_STATIC_DRAW);
    }

    if (hasTexCoords()) {
        glGenBuffers(1, &texCoordVbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        glBufferData(GL_ARRAY_BUFFER,
                     vertexCount * sizeof(float) * 2,
                     (GLfloat*) texCoords.get(),
                     GL_STATIC_DRAW);
    }

    glGenBuffers(1, &indexVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indexCount * sizeof(int),
                 (GLuint*) indices.get(),
                 GL_STATIC_DRAW);
}

bool Mesh::hasVbos() const {
    return vertexVbo != 0;
}

int Mesh::getNumVertices() const {
    return vertexCount;
}

shared_array<Vector> Mesh::getVertices() {
    return vertices;
}

GLuint Mesh::getVertexVbo() const {
    return vertexVbo;
}

bool Mesh::hasNormals() const {
    return normals.get() != NULL;
}

GLuint Mesh::getNormalVbo() const {
    return normalVbo;
}

shared_array<Vector> Mesh::getNormals() {
    return normals;
}

bool Mesh::hasTexCoords() const {
    return texCoords.get() != NULL;
}

shared_array<Pair> Mesh::getTexCoords() {
    return texCoords;
}

GLuint Mesh::getTexCoordVbo() const {
    return texCoordVbo;
}

int Mesh::getNumTriangles() const {
    return indexCount / 3;
}

shared_array<int> Mesh::getIndices() {
    return indices;
}

GLuint Mesh::getIndexVbo() const {
    return indexVbo;
}
