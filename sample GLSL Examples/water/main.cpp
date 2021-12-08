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

#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <fpu_control.h>
#include <fenv.h>
#include <cmath>
#include "GLView.h"
#include "Controller.h"
#include "Utilities.h"

static void display();
static void reshape(int width, int height);
static void keyboard(unsigned char key, int x, int y);
static void keyboardUp(unsigned char key, int x, int y);
static void special(int key, int x, int y);
static void specialUp(int key, int x, int y);
static void mouse(int button, int state, int x, int y);
static void motion(int x, int y);
static void wheel(int button, int direction, int x, int y);
static void idle();

int main(int argc, char **argv) {
    feenableexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW);
    srand((int) getTime());
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("glut");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMouseWheelFunc(wheel);
    glutIdleFunc(idle);
    controller.initialize();
    glview.initialize();
    return 0;
}

static void display() {
    glview.display();
}

static void reshape(int width, int height) {
    glview.reshape(width, height);
}

static void keyboard(unsigned char ch, int x, int y) {
    controller.keyboardDown(ch, x, y);
}

static void keyboardUp(unsigned char ch, int x, int y) {
    controller.keyboardUp(ch, x, y);
}

static void special(int key, int x, int y) {
    controller.specialDown(key, x, y);
}

static void specialUp(int key, int x, int y) {
    controller.specialUp(key, x, y);
}

static void mouse(int button, int state, int x, int y) {
    controller.mouse(button, state, x, y);
}

static void motion(int x, int y) {
    controller.motion(x, y);
}

static void wheel(int button, int direction, int x, int y) {
    controller.wheel(button, direction, x, y);
}

static void idle() {
    controller.update();
}
