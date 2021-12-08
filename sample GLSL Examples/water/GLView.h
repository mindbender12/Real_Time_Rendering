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

#ifndef GLView_h
#define GLView_h

#include <vector>
#include <boost/shared_ptr.hpp>

class Camera;
class ParentDrawable;
class GUIDrawable;
class Light;

class GLView {
    // Constructors
    public:
    GLView(int width, int height);

    // Methods
    public:
    void initialize();

    void initGL();
    void reshape(int width, int height);
    void display();

    int getWidth();
    int getHeight();
    boost::shared_ptr<Camera> getCamera();
    void setCamera(boost::shared_ptr<Camera> camera);

    // Fields
    private:
    int width, height;
    boost::shared_ptr<Camera> camera;

    public:
    boost::shared_ptr<ParentDrawable> scene;
    boost::shared_ptr<GUIDrawable> gui;

    std::vector<boost::shared_ptr<Light> > lights;
};

extern GLView glview;

#endif
