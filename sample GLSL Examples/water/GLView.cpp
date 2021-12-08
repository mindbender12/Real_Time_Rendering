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

#include "GLView.h"

#include <vector>
#include <cstdlib>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <boost/shared_ptr.hpp>
#include "Vector.h"
#include "Quaternion.h"
#include "PositionedCamera.h"
#include "ParentDrawable.h"
#include "GUIDrawable.h"
#include "Light.h"

#warning debug
#include "Mesh.h"
#include "MeshDrawable.h"

using namespace std;
using namespace boost;

GLView glview(800, 600);

#include "Font.h"

static shared_ptr<Font> font;

GLView::GLView(int width, int height) :
    width(width),
    height(height),
    camera(new PositionedCamera),
    scene(new ParentDrawable),
    gui(new GUIDrawable)
{
}

void GLView::initialize() {
    font.reset(new Font("fonts/arial.ttf", 32.));
    initGL();
    glutMainLoop();
}

static shared_ptr<Mesh> mesh;
static shared_ptr<MeshDrawable> view;

void GLView::initGL() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLView::reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,
                   (double) width / (double) height,
                   0.1,
                   1000.0);

    glMatrixMode(GL_MODELVIEW);
}

void GLView::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera->set();
    if (!lights.empty()) {
        glEnable(GL_LIGHTING);
        for (int i = 0; i < lights.size(); ++i) {
            glEnable(GL_LIGHT0 + i);
            lights[i]->set(i);
        }
    } else
        glDisable(GL_LIGHTING);
    scene->draw();
    gui->draw();

    glutSwapBuffers();
    glutPostRedisplay();
}

int GLView::getWidth() {
    return width;
}

int GLView::getHeight() {
    return height;
}

shared_ptr<Camera> GLView::getCamera() {
    return camera;
}

void GLView::setCamera(shared_ptr<Camera> camera) {
    this->camera = camera;
}
