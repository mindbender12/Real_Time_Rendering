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

#include "Terrain.h"

#include <cassert>
#include <CImg.h>
#include "Mesh.h"
#include "Vector.h"
#include "Utilities.h"

using namespace std;
using namespace boost;
using namespace cimg_library;

Terrain::Terrain(int rows, int columns, 
                 float horizontalScale, float verticalScale,
                 float waterHeight,
                 shared_array<float> points) :
    rows(rows),
    columns(columns),
    horizontalScale(horizontalScale),
    verticalScale(verticalScale),
    waterHeight(waterHeight),
    points(points)
{
    int vertexCount = rows * columns;
    shared_array<Vector> vertices(new Vector[vertexCount]);
    shared_array<Pair> texCoords(new Pair[vertexCount]);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            int i = r * rows + c;
            float x = horizontalScale * c;
            float y = horizontalScale * r;
            float z = verticalScale * points[i];
            vertices[i] = Vector(x, y, z);
            texCoords[i] = Pair(x/2, y/2);
        }
    }

    int indexCount = (rows - 1) * (columns - 1) * 6;
    shared_array<int> indices(new int[indexCount]);
    for (int r = 0; r < rows - 1; ++r) {
        for (int c = 0; c < columns - 1; ++c) {
            int i = r * (rows - 1) + c;
            indices[6*i + 0] = r * rows + c;
            indices[6*i + 1] = r * rows + c + 1;
            indices[6*i + 2] = (r + 1) * rows + c;
            indices[6*i + 3] = (r + 1) * rows + c;
            indices[6*i + 4] = r * rows + c + 1;
            indices[6*i + 5] = (r + 1) * rows + c + 1;
        }
    }

    mesh.reset(new Mesh(vertexCount, vertices, 
                        shared_array<Vector>(), texCoords,
                        indexCount, indices));
    mesh->generateNormals();
    mesh->generateVbos();

    generateWater();
}

shared_ptr<Mesh> Terrain::getMesh() {
    return mesh;
}

shared_ptr<Mesh> Terrain::getWaterMesh() {
    return water;
}

int Terrain::getRows() const {
    return rows;
}

int Terrain::getColumns() const {
    return columns;
}

float Terrain::getHorizontalScale() const {
    return horizontalScale;
}

float Terrain::getVerticalScale() const {
    return verticalScale;
}

float Terrain::getWaterHeight() const {
    return waterHeight;
}

float Terrain::getSizeX() const {
    return (columns - 1) * horizontalScale;
}

float Terrain::getSizeY() const {
    return (rows - 1) * horizontalScale;
}

float Terrain::getHeightAt(int r, int c) const {
    assert(0 <= r && r < rows);
    assert(0 <= c && c < columns);
    return points[r*columns + c] * verticalScale;
}

float Terrain::getHeightAt(float x, float y) const {
    assert(0.0 <= x && x < getSizeX());
    assert(0.0 <= y && y < getSizeY());
    int row = (int) (y / horizontalScale);
    int column = (int) (x / horizontalScale);
    assert(row < rows - 1 && column < columns - 1);
    float a = (x / horizontalScale) - column;
    float b = (y / horizontalScale) - row;
    float height = 0.0;
    height += (1.0 - a) * (1.0 - b) * points[row*columns + column];
    height += a * (1.0 - b) * points[row*columns + column + 1];
    height += a * b * points[(row+1)*columns + column + 1];
    height += (1.0 - a) * b * points[(row+1)*columns + column];
    height *= verticalScale;
    return height;
}

shared_ptr<Terrain> Terrain::fromFile(const string &filename, 
                                      float horizontalScale, 
                                      float verticalScale,
                                      float waterHeight) 
{
    CImg<unsigned char> image(filename.c_str());
    int columns = image.width(), rows = image.height();
    int channels = image.spectrum();
    shared_array<float> points(new float[rows * columns]);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            int r = rows - i - 1;
            int c = j;
            float height = 0;
            for (int k = 0; k < channels; ++k) {
                int index = i*columns*channels + j*channels + k;
                height += (float) image(j, i, 0, k) / 255.0 / channels;
            }
            points[r*columns + c] = height;
        }
    }
    shared_ptr<Terrain> terrain(new Terrain(rows, columns, horizontalScale, verticalScale, waterHeight, points));
    return terrain;
}

void Terrain::generateWater() {
    int numSquaresBelowWater = 0;
    for (int r = 0; r < rows - 1; ++r) {
        for (int c = 0; c < columns - 1; ++c) {
            if (squareIsBelowWater(r, c))
                ++numSquaresBelowWater;
        }
    }

    int vertexCount = numSquaresBelowWater * 6;
    int indexCount = vertexCount;
    shared_array<Vector> vertices(new Vector[vertexCount]);
    shared_array<int> indices(new int[indexCount]);

    int vi = 0, ii = 0;
    for (int r = 0; r < rows - 1; ++r) {
        for (int c = 0; c < columns - 1; ++c) {
            if (squareIsBelowWater(r, c)) {
                vertices[vi++] = vectorForCoordinates(r, c) + Vector(0, 0, 1);
                vertices[vi++] = vectorForCoordinates(r, c + 1) + Vector(0, 0, 1);
                vertices[vi++] = vectorForCoordinates(r + 1, c) + Vector(0, 0, 1);
                vertices[vi++] = vectorForCoordinates(r + 1, c) + Vector(0, 0, 1);
                vertices[vi++] = vectorForCoordinates(r, c + 1) + Vector(0, 0, 1);
                vertices[vi++] = vectorForCoordinates(r + 1, c + 1) + Vector(0, 0, 1);
                for (int j = 0; j < 6; ++j) {
                    indices[ii] = ii;
                    ++ii;
                }
            }
        }
    }
    assert(vi == vertexCount && ii == indexCount);

    water.reset(new Mesh(vertexCount, vertices, 
                         shared_array<Vector>(), shared_array<Pair>(), 
                         indexCount, indices));
    water->generateVbos();    
}

bool Terrain::squareIsBelowWater(int r, int c) {
    return getHeightAt(r, c) < waterHeight ||
        getHeightAt(r + 1, c) < waterHeight ||
        getHeightAt(r + 1, c + 1) < waterHeight ||
        getHeightAt(r, c + 1) < waterHeight;
}

Vector Terrain::vectorForCoordinates(int r, int c) {
    return Vector(c * horizontalScale, r * horizontalScale, getHeightAt(r, c));
}

