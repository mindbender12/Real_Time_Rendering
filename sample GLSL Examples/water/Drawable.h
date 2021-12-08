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

#ifndef Drawable_h
#define Drawable_h

#include "Vector.h"
#include "Quaternion.h"

class Drawable {
    // Constructors
    public:
    Drawable();
    Drawable(const Vector &position, const Quaternion &orientation);
    virtual ~Drawable();

    // Methods
    public:
    void draw();

    Vector getPosition() const;
    void setPosition(const Vector &v);
    Quaternion getOrientation() const;
    void setOrientation(const Quaternion &q);

    protected:
    virtual void drawSelf() = 0;

    // Fields
    protected:
    Vector position;
    Quaternion orientation;
};

#endif
