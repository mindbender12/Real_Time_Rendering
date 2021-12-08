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

#ifndef FocusedCamera_h
#define FocusedCamera_h

#include "Camera.h"
#include "Vector.h"

class FocusedCamera : public Camera {
    // Constructors
    public:
    FocusedCamera();

    // Methods
    public:

    void set();
    Vector getPosition() const;

    Vector getFocus() const;
    void setFocus(Vector focus);
    float getRadius() const;
    void setRadius(float radius);
    float getZenith() const;
    void setZenith(float zenith);
    float getAzimuth() const;
    void setAzimuth(float azimuth);

    // Fields
    private:
    Vector focus;
    float radius, zenith, azimuth;
};

#endif
