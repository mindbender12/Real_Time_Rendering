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

#include "Shapes.h"

#include <boost/shared_array.hpp>
#include <cassert>
#include <cmath>
#include "Mesh.h"
#include "Vector.h"
#include "Utilities.h"

using namespace boost;

shared_ptr<Mesh> makeSquare(float size) {
    int vertexCount = 4;
    shared_array<Vector> vertices(new Vector[vertexCount]);
    shared_array<Vector> normals(new Vector[vertexCount]);
    shared_array<Pair> texCoords(new Pair[vertexCount]);

    vertices[0] = Vector(-size/2, size/2, 0.0);
    vertices[1] = Vector(-size/2, -size/2, 0.0);
    vertices[2] = Vector(size/2, -size/2, 0.0);
    vertices[3] = Vector(size/2, size/2, 0.0);

    for (int i = 0; i < vertexCount; ++i)
        normals[i] = Vector(0.0, 0.0, 1.0);

    texCoords[0] = Pair(0.0, 1.0);
    texCoords[1] = Pair(0.0, 0.0);
    texCoords[2] = Pair(1.0, 0.0);
    texCoords[3] = Pair(1.0, 1.0);

    int indexCount = 6;
    shared_array<int> indices(new int[indexCount]);
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;

    shared_ptr<Mesh> mesh(new Mesh(vertexCount, vertices, normals, texCoords, indexCount, indices));
    return mesh;
}

shared_ptr<Mesh> makeCylinder(float radius, float height, int slices) {
    assert(slices >= 3);

    int vertexCount = slices * 2;
    shared_array<Vector> vertices(new Vector[vertexCount]);
    shared_array<Vector> normals(new Vector[vertexCount]);
    shared_array<Pair> texCoords(new Pair[vertexCount]);

    for (int i = 0; i < 2; ++i) {
        float z = i == 0 ? height/2 : -height/2;
        for (int j = 0; j < slices; ++j) {
            float angle = 2 * M_PI * (float) j / (float) slices;
            float x = radius * cosf(angle);
            float y = radius * sinf(angle);
            Vector v(x, y, z);
            vertices[i*slices + j] = v;

            Vector n(cosf(angle), sinf(angle), 0.0);
            normals[i*slices + j] = n;
            Pair t((float) j / (float) slices, i == 0 ? 0.0 : 1.0);
            texCoords[i*slices + j] = t;
        }
    }

    int indexCount = slices * 6;
    shared_array<int> indices(new int[indexCount]);
    for (int i = 0; i < slices; ++i) {
        int a = i;
        int b = slices + i;
        int c = slices + (i + 1) % slices;
        int d = (i + 1) % slices;
        indices[i*6 + 0] = a;
        indices[i*6 + 1] = b;
        indices[i*6 + 2] = c;
        indices[i*6 + 3] = a;
        indices[i*6 + 4] = c;
        indices[i*6 + 5] = d;
    }

    shared_ptr<Mesh> mesh(new Mesh(vertexCount, vertices, normals, texCoords, indexCount, indices));
    return mesh;
}
