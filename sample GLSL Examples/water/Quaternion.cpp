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

#include "Quaternion.h"

#include <cmath>
#include "Vector.h"

#define TOLERANCE 1e-6

using namespace std;

Quaternion::Quaternion() :
_w(1.0),
_x(0.0),
_y(0.0),
_z(0.0)
{}

Quaternion::Quaternion(float w, float x, float y, float z) :
_w(w),
_x(x),
_y(y),
_z(z)
{}

Quaternion::Quaternion(float angle, Vector axis) :
_w(cos(angle/2.)),
_x(sin(angle/2.) * axis.x()),
_y(sin(angle/2.) * axis.y()),
_z(sin(angle/2.) * axis.z())
{}

Quaternion::Quaternion(float pitch, float yaw, float roll)
{
    float sinp = sinf(pitch/2);
    float siny = sinf(yaw/2);
    float sinr = sinf(roll/2);
    float cosp = cosf(pitch/2);
    float cosy = cosf(yaw/2);
    float cosr = cosf(roll/2);
    
    _w = cosr * cosp * cosy + sinr * sinp * siny;
    _x = sinr * cosp * cosy - cosr * sinp * siny;
    _y = cosr * sinp * cosy + sinr * cosp * siny;
    _z = cosr * cosp * siny - sinr * sinp * cosy;
}

float Quaternion::w() const
{
    return _w;
}

float Quaternion::x() const
{
    return _x;
}

float Quaternion::y() const
{
    return _y;
}

float Quaternion::z() const
{
    return _z;
}

float Quaternion::angle() const
{
    if (_w < -1.0 || _w > 1.0)
        return 0.0;
    else
        return acos(_w) * 2.;
}

Vector Quaternion::axis() const
{
    float scale = sqrt(_x * _x + _y * _y + _z * _z);
    if (scale == 0.0)
        return Vector(1.0, 0.0, 0.0);
    else
        return Vector(_x / scale, _y / scale, _z / scale);
}

float Quaternion::magnitude() const
{
    return sqrtf(_w * _w + _x * _x + _y * _y + _z * _z);
}

Quaternion Quaternion::normalize() const
{
    float reciprocal = 1. / magnitude();
    return Quaternion(_w * reciprocal,
                      _x * reciprocal,
                      _y * reciprocal,
                      _z * reciprocal);
}

Quaternion Quaternion::conjugate() const
{
    return Quaternion(_w, -_x, -_y, -_z);
}

Quaternion Quaternion::operator * (const Quaternion &q) const
{
    return Quaternion(_w * q._w - _x * q._x - _y * q._y - _z * q._z,
                      _w * q._x + _x * q._w + _y * q._z - _z * q._y,
                      _w * q._y + _y * q._w + _z * q._x - _x * q._z,
                      _w * q._z + _z * q._w + _x * q._y - _y * q._x);
}

Vector Quaternion::operator * (const Vector &v) const
{
    Vector vn = v.normalize();
    Quaternion vq(0.0, vn.x(), vn.y(), vn.z());
    Quaternion rq = *this * vq * conjugate();
    return Vector(rq._x, rq._y, rq._z);
}

void Quaternion::getMatrix(float mat[16]) const
{
    float x2 = _x * _x;
    float y2 = _y * _y;
    float z2 = _z * _z;
    float xy = _x * _y;
    float xz = _x * _z;
    float yz = _y * _z;
    float wx = _w * _x;
    float wy = _w * _y;
    float wz = _w * _z;
    
    mat[0] = 1. - 2.*y2 - 2.*z2;
    mat[1] = 2.*xy + 2.*wz;
    mat[2] = 2.*xz - 2.*wy;
    mat[3] = 0.;
    
    mat[4] = 2.*xy - 2.*wz;
    mat[5] = 1. - 2.*x2 - 2.*z2;
    mat[6] = 2.*yz - 2.*wx;
    mat[7] = 0.;
    
    mat[8] = 2.*xz + 2.*wy;
    mat[9] = 2.*yz - 2.*wx;
    mat[10] = 1. - 2.*x2 -2.*y2;
    mat[11] = 0.;
    
    mat[12] = 0.;
    mat[13] = 0.;
    mat[14] = 0.;
    mat[15] = 1.;
}

std::ostream& operator << (ostream &os, Quaternion q) {
    os << q.w() << " + i*" << q.x() << " + j*" << q.y() << " + k*" << q.z();
    return os;
}
