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

#ifndef Controller_h
#define Controller_h

class GLView;

class Controller {
    // Methods
    public:
    void initialize();

    double getElapsedTime();

    void update();
    void keyboardDown(char ch, int x, int y);
    void keyboardUp(char ch, int x, int y);
    void specialDown(int key, int x, int y);
    void specialUp(int key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void wheel(int button, int direction, int x, int y);
    void motion(int x, int y);

    // Fields
    private:
    GLView *view;
    double startingTime;
    double elapsedTime;
};

extern Controller controller;

#endif
