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

#include "GUIDrawable.h"

#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;
using namespace boost;

GUIDrawable::GUIDrawable() {}

GUIDrawable::GUIDrawable(const vector<shared_ptr<Drawable> > &children) :
    ParentDrawable(children)
{
}

void GUIDrawable::drawSelf() {
    glPushAttrib(GL_TRANSFORM_BIT);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    drawChildren();

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPopAttrib();
}
