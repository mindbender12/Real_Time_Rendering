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

#include "Drawable.h"

#include <cmath>
#include <GL/gl.h>

Drawable::Drawable() {}

Drawable::Drawable(const Vector &position, const Quaternion &orientation) :
    position(position),
    orientation(orientation)
{
}

Drawable::~Drawable() {}

void Drawable::draw() {
    glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    float angle = orientation.angle();
    Vector axis = orientation.axis();
    glRotatef(angle * 180./M_PI, axis.x(), axis.y(), axis.z());
    drawSelf();
    glPopMatrix();
}

Vector Drawable::getPosition() const {
    return position;
}

void Drawable::setPosition(const Vector &v) {
    position = v;
}

Quaternion Drawable::getOrientation() const {
    return orientation;
}

void Drawable::setOrientation(const Quaternion &q) {
    orientation = q;
}
