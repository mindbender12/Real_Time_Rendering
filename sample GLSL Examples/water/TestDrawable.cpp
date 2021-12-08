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

#include "TestDrawable.h"

#include <GL/gl.h>

void TestDrawable::drawSelf() {
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5, -1.0);
    glVertex2f(0.5, -1.0);
    glVertex2f(0.0, 1.0);
    glEnd();
}
