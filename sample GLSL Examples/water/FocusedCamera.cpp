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

#include "FocusedCamera.h"

#include <cmath>
#include <cassert>
#include <GL/gl.h>
#include <GL/glu.h>

FocusedCamera::FocusedCamera() :
    radius(10.0),
    zenith(M_PI/4.0),
    azimuth(0.0)
{
}

void FocusedCamera::set() {
    Vector position = getPosition();
    gluLookAt(position.x(), position.y(), position.z(),
              focus.x(), focus.y(), focus.z(),
              0.0, 0.0, 1.0);
}

Vector FocusedCamera::getPosition() const {
    Vector position(focus.x() + radius * cos(azimuth) * sin(zenith),
                    focus.y() + radius * sin(azimuth) * sin(zenith),
                    focus.z() + radius * cos(zenith));
    return position;
}

Vector FocusedCamera::getFocus() const {
    return focus;
}

void FocusedCamera::setFocus(Vector focus) {
    this->focus = focus;
}

float FocusedCamera::getRadius() const {
    return radius;
}

void FocusedCamera::setRadius(float radius) {
    assert(radius > 0.0);
    this->radius = radius;
}

float FocusedCamera::getZenith() const {
    return zenith;
}

void FocusedCamera::setZenith(float zenith) {
    assert(-M_PI/2 < zenith && zenith < M_PI/2);
    this->zenith = zenith;
}

float FocusedCamera::getAzimuth() const {
    return azimuth;
}

void FocusedCamera::setAzimuth(float azimuth) {
    this->azimuth = azimuth;
}
