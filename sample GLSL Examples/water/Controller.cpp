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

#include "Controller.h"

#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include <sys/time.h>
#include <cmath>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include "GLView.h"
#include "Vector.h"
#include "Quaternion.h"
#include "MeshDrawable.h"
#include "Utilities.h"
#include "FocusedCamera.h"
#include "Terrain.h"
#include "ParentDrawable.h"
#include "Texture.h"
#include "TestDrawable.h"
#include "Light.h"
#include "Font.h"
#include "GUIDrawable.h"
#include "TextDrawable.h"
#include "Shader.h"
#include "CubeMap.h"

using namespace std;
using namespace boost;

extern GLView glview;

Controller controller;

static const float SCROLL_INCREMENT = 1.0;
static const float ROTATE_INCREMENT = 0.01;
static const float ZOOM_INCREMENT = 1.0;
static bool leftPressed = false;
static bool rightPressed = false;
static bool upPressed = false;
static bool downPressed = false;
static int mouseX = -1;
static int mouseY = -1;
static int frames = 0;

shared_ptr<Terrain> terrain;
shared_ptr<TextDrawable> text;

void Controller::initialize() {
    view = &glview;
    startingTime = ::getTime();
    elapsedTime = 0.0;

    terrain = Terrain::fromFile("textures/heightmap.jpg", 1.0, 100.0, 20.0);
    shared_ptr<Mesh> mesh = terrain->getMesh();
    shared_ptr<MeshDrawable> terrainView(new MeshDrawable(mesh));
    shared_ptr<Texture> dirt = Texture::fromFile("textures/sand.png");
    terrainView->setTexture(dirt);
    view->scene->addChild(terrainView);

    shared_ptr<Mesh> water = terrain->getWaterMesh();
    shared_ptr<MeshDrawable> waterView(new MeshDrawable(water));
    shared_ptr<Shader> waterShader = Shader::fromFiles("shaders/water.vs", "shaders/water.fs");
    waterShader->setUniform("waterHeight", terrain->getWaterHeight());
    waterShader->setUniform("numWaves", 4);
    waterShader->setUniform("envMap", 0);
    for (int i = 0; i < 4; ++i) {
        float amplitude = 0.5f / (i + 1);
        waterShader->setUniform(format("amplitude[%d]", i), amplitude);

        float wavelength = 8 * M_PI / (i + 1);
        waterShader->setUniform(format("wavelength[%d]", i), wavelength);

        float speed = 1.0f + 2*i;
        waterShader->setUniform(format("speed[%d]", i), speed);

        float angle = uniformRandomInRange(-M_PI/3, M_PI/3);
        waterShader->setUniform(format("direction[%d]", i), cos(angle), sin(angle));
    }
    waterView->setShader(waterShader);
    string cubeMapFilenames[6] = {
        "textures/lostvalley_east.bmp",
        "textures/lostvalley_west.bmp",
        "textures/lostvalley_up.bmp",
        "textures/lostvalley_down.bmp",
        "textures/lostvalley_north.bmp",
        "textures/lostvalley_south.bmp"
    };
    shared_ptr<CubeMap> cubeMap = CubeMap::fromFiles(cubeMapFilenames);
    waterView->setCubeMap(cubeMap);
    waterView->setBlend(true);
    view->scene->addChild(waterView);

    shared_ptr<FocusedCamera> camera(new FocusedCamera);
    float cameraX = terrain->getSizeX()/2;
    float cameraY = terrain->getSizeY()/2;
    float cameraZ = terrain->getHeightAt(cameraX, cameraY);
    camera->setFocus(Vector(cameraX, cameraY, cameraZ));
    camera->setRadius(10.0);
    camera->setAzimuth(3*M_PI/2);
    camera->setZenith(M_PI/4);
    view->setCamera(camera);

    shared_ptr<Light> light(new Light(true, Vector(0.0, 0.0, 1.0),
                                      Color(0.2, 0.2, 0.2, 1.0),
                                      Color(1.0, 1.0, 1.0, 1.0)));
    view->lights.push_back(light);

    shared_ptr<Font> font(new Font("fonts/arial.ttf", 16));
    text.reset(new TextDrawable("GLUT Test", font, Color(1.0, 1.0, 1.0, 1.0)));
    view->gui->addChild(text);
}

double Controller::getElapsedTime() {
    return elapsedTime;
}

void Controller::update() {
    ++frames;
    double t = getTime();
    if ((int) (t - startingTime) != (int) elapsedTime) {
        stringstream ss;
        ss << "FPS: " << frames;
        text->setText(ss.str());
        frames = 0;
    }
    double deltaT = t - (startingTime + elapsedTime);
    elapsedTime = t - startingTime;
    shared_ptr<FocusedCamera> camera = dynamic_pointer_cast<FocusedCamera>(view->getCamera());
    float radius = camera->getRadius();
    
    float dx = 0, dy = 0;
    float theta = camera->getAzimuth() - 3*M_PI/2;
    if (rightPressed) {
        dx += cos(theta);
        dy += sin(theta);
    }
    if (leftPressed) {
        dx += cos(theta + M_PI);
        dy += sin(theta + M_PI);
    }
    if (upPressed) {
        dx += cos(theta + M_PI/2);
        dy += sin(theta + M_PI/2);
    }
    if (downPressed) {
        dx += cos(theta + 3*M_PI/2);
        dy += sin(theta + 3*M_PI/2);
    }
    Vector delta = Vector(dx, dy, 0.0) * SCROLL_INCREMENT * radius * deltaT;
    Vector focus = camera->getFocus();
    float boundX = terrain->getSizeX();
    float boundY = terrain->getSizeY();
    float focusX = clamp(0.0, focus.x() + delta.x(), boundX - 0.01);
    float focusY = clamp(0.0, focus.y() + delta.y(), boundY - 0.01);
    float focusZ = terrain->getHeightAt(focusX, focusY); 
    focus = Vector(focusX, focusY, focusZ);
    camera->setFocus(focus);
}

void Controller::keyboardDown(char ch, int x, int y) {
    if (ch == 'q' || ch == 0x1B)
        exit(0);
}

void Controller::keyboardUp(char ch, int x, int y) {
}

void Controller::specialDown(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT)
        leftPressed = true;
    else if (key == GLUT_KEY_RIGHT)
        rightPressed = true;
    else if (key == GLUT_KEY_UP)
        upPressed = true;
    else if (key == GLUT_KEY_DOWN)
        downPressed = true;
 }

void Controller::specialUp(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT)
        leftPressed = false;
    else if (key == GLUT_KEY_RIGHT)
        rightPressed = false;
    else if (key == GLUT_KEY_UP)
        upPressed = false;
    else if (key == GLUT_KEY_DOWN)
        downPressed = false;
}

void Controller::mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseX = x;
            mouseY = y;
        } else {
            mouseX = -1;
            mouseY = -1;
        }
    } else if (button == 3)
        wheel(0, 1, x, y);
    else
        wheel(0, -1, x, y);
}

void Controller::motion(int x, int y) {
    if (mouseX != -1 && mouseY != -1) {
        int dx = x - mouseX;
        int dy = y - mouseY;
        shared_ptr<FocusedCamera> camera = static_pointer_cast<FocusedCamera>(view->getCamera());
        float azimuth = camera->getAzimuth() - dx * ROTATE_INCREMENT;
        camera->setAzimuth(azimuth);
        float zenith = camera->getZenith() - dy * ROTATE_INCREMENT;
        if (zenith < M_PI/40)
            zenith = M_PI/40;
        if (zenith > 19*M_PI/40)
            zenith = 19*M_PI/40;
        camera->setZenith(zenith);
        mouseX = x;
        mouseY = y;
    }
}

void Controller::wheel(int button, int direction, int y, int z) {
    shared_ptr<FocusedCamera> camera = static_pointer_cast<FocusedCamera>(view->getCamera());
    float radius = camera->getRadius();
    float zoom = sqrt(radius);
    zoom -= direction * ZOOM_INCREMENT;
    radius = zoom * zoom;
    if (radius < 0.1)
        radius = 0.1;
    camera->setRadius(radius);
}

