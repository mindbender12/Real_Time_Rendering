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

#include "PositionedCamera.h"

#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>

void PositionedCamera::set() {
    glLoadIdentity();
    float angle = orientation.angle();
    Vector axis = orientation.axis();
    glRotatef(-angle * 180./M_PI, axis.x(), axis.y(), axis.z());
    glTranslatef(-position.x(), -position.y(), -position.z());
}

Vector PositionedCamera::getPosition() const {
    return position;
}

void PositionedCamera::setPosition(Vector position) {
    this->position = position;
}

Quaternion PositionedCamera::getOrientation() const {
    return orientation;
}

void PositionedCamera::setOrientation(Quaternion orientation) {
    this->orientation = orientation;
}
