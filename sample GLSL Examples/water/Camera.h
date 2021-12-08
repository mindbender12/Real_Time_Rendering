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

#ifndef Camera_h
#define Camera_h

class Vector;

class Camera {
    public:
    virtual void set() = 0;
    virtual Vector getPosition() const = 0;
};

#endif
