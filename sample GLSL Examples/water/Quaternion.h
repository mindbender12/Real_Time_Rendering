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

#ifndef Quaternion_h
#define Quaternion_h

#include <iostream>

class Vector;

class Quaternion {
public:
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(float angle, Vector axis);
    Quaternion(float pitch, float yaw, float roll);
    
    float w() const;
    float x() const;
    float y() const;
    float z() const;
    
    float angle() const;
    Vector axis() const;

    float magnitude() const;
    Quaternion normalize() const;
    Quaternion conjugate() const;
    Quaternion operator * (const Quaternion &q) const;
    Vector operator * (const Vector &v) const;
    
    void getMatrix(float mat[16]) const;
    
private:
    float _w, _x, _y, _z;
};

std::ostream& operator << (std::ostream &os, Quaternion q);

#endif
