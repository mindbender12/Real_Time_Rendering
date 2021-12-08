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

#ifndef PositionedCamera_h
#define PositionedCamera_h

#include "Vector.h"
#include "Quaternion.h"
#include "Camera.h"

class PositionedCamera : public Camera {
    // Methods
    public:

    void set();

    Vector getPosition() const;
    void setPosition(Vector position);
    Quaternion getOrientation() const;
    void setOrientation(Quaternion orientation);

    // Fields
    private:

    Vector position;
    Quaternion orientation;
};

#endif
