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

#ifndef Light_h
#define Light_h

#include "Vector.h"
#include "Utilities.h"

class Light {
    // Constructor
    public:

    Light(bool directional, Vector position, Color ambient, Color diffuse);

    // Methods
    public:

    void set(int id);

    bool isDirectional() const;
    void setDirectional(bool directional);
    Vector getPosition() const;
    void setPosition(Vector position);
    Color getAmbientColor() const;
    void setAmbientColor(Color ambient);
    Color getDiffuseColor() const;
    void setDiffuseColor(Color diffuse);

    // Fields
    private:
    bool directional;
    Vector position;
    Color ambient;
    Color diffuse;
};

#endif
