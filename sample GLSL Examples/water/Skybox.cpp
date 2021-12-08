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

#include "Skybox.h"

#include <cmath>
#include <GL/gl.h>
#include "Texture.h"
#include "Vector.h"
#include "Quaternion.h"
#include "GLView.h"
#include "Camera.h"

using namespace boost;

Skybox::Skybox(shared_ptr<Texture> north,
               shared_ptr<Texture> east,
               shared_ptr<Texture> south,
               shared_ptr<Texture> west,
               shared_ptr<Texture> up,
               shared_ptr<Texture> down) :
    north(north),
    east(east),
    south(south),
    west(west),
    up(up),
    down(down)
{
}

void Skybox::drawSelf() {
    /*
    Quaternion cameraOrientation(glview.getCamera()->getOrientation());
    glPushMatrix();
    glLoadIdentity();
    float angle = cameraOrientation.angle() * 180./M_PI;
    Vector axis = cameraOrientation.axis();
    glRotatef(-angle, axis.x(), axis.y(), axis.z());
    */

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    shared_ptr<Texture> textures[] = { north, east, south, west, up, down };
    Quaternion orientations[] = {
        Quaternion(),
        Quaternion(3*M_PI/2, 0.0, 0.0),
        Quaternion(M_PI, 0.0, 0.0),
        Quaternion(M_PI/2, 0.0, 0.0),
        Quaternion(M_PI, 0.0, M_PI/2),
        Quaternion(M_PI, 0.0, -M_PI/2)
    };

    float mat[16];
    for (int i = 0; i < 6; ++i) {
        textures[i]->bind();

        glPushMatrix();
        orientations[i].getMatrix(mat);
        glMultMatrixf(mat);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-1.0, 1.0, -1.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(1.0, -1.0, -1.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glEnd();

        glPopMatrix();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}
