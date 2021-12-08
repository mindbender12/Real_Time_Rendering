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

#ifndef Terrain_h
#define Terrain_h

#include <string>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

class Mesh;
class Vector;

class Terrain {
    // Constructors
    public:

    Terrain(int rows, int columns, float horizontalScale, float verticalScale, 
            float waterHeight,
            boost::shared_array<float> points);

    // Methods
    public:

    boost::shared_ptr<Mesh> getMesh();
    boost::shared_ptr<Mesh> getWaterMesh();
    int getRows() const;
    int getColumns() const;
    float getHorizontalScale() const;
    float getVerticalScale() const;
    float getWaterHeight() const;
    float getSizeX() const;
    float getSizeY() const;
    float getHeightAt(int r, int c) const;
    float getHeightAt(float x, float y) const;

    static boost::shared_ptr<Terrain> fromFile(const std::string &filename,
                                               float horizontalScale,
                                               float verticalScale,
                                               float waterHeight);

    private:
    void generateWater();
    bool squareIsBelowWater(int r, int c);
    Vector vectorForCoordinates(int r, int c);

    // Fields
    private:
    int rows, columns;
    float horizontalScale;
    float verticalScale;
    float waterHeight;
    boost::shared_array<float> points;
    boost::shared_ptr<Mesh> mesh;
    boost::shared_ptr<Mesh> water;
};

#endif
