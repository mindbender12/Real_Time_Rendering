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

#include "Light.h"

#include <GL/gl.h>

Light::Light(bool directional, Vector position, Color ambient, Color diffuse) :
    directional(directional),
    position(position),
    ambient(ambient),
    diffuse(diffuse)
{
}

void Light::set(int id) {
    float pos[4] = { position.x(), position.y(), position.z(), directional ? 0.0 : 1.0 };
    glLightfv(GL_LIGHT0 + id, GL_POSITION, pos);
    glLightfv(GL_LIGHT0 + id, GL_AMBIENT, ambient.array());
    glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, diffuse.array());
}

bool Light::isDirectional() const {
    return directional;
}

void Light::setDirectional(bool directional) {
    this->directional = directional;
}

Vector Light::getPosition() const {
    return position;
}

void Light::setPosition(Vector position) {
    this->position = position;
}

Color Light::getAmbientColor() const {
    return ambient;
}

void Light::setAmbientColor(Color ambient) {
    this->ambient = ambient;
}

Color Light::getDiffuseColor() const {
    return diffuse;
}

void Light::setDiffuseColor(Color diffuse) {
    this->diffuse = diffuse;
}

