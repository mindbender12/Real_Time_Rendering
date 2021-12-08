#include <cmath>
#include <cstring>
#include <iostream>
#include "Vector.h"

using std::cout;
using std::endl;


Vector::Vector()
{
	SetNull();
}


Vector::Vector(float v0, float v1, float v2)
{
	Load(v0, v1, v2);
}


Vector::Vector(float *vec)
{
	Load(vec);
}


void Vector::Load(float v0, float v1, float v2)
{
	_array[0] = v0;
	_array[1] = v1;
	_array[2] = v2;
}


void Vector::Load(float *vec)
{
	_array[0] = vec[0];
	_array[1] = vec[1];
	_array[2] = vec[2];
}


float Vector::Length() const
{
	return sqrt(_array[0]*_array[0] + _array[1]*_array[1] + _array[2]*_array[2]);
}


Vector& Vector::Normalize()
{
	float len = Length();

	_array[0] /= len;
	_array[1] /= len;
	_array[2] /= len;

	return *this;
}


bool Vector::operator==(const Vector& vec) const
{
	return (_array[0] == vec._array[0] &&
	        _array[1] == vec._array[1] &&
	        _array[2] == vec._array[2]);
}


Vector Vector::operator+(const Vector& vec) const
{
	Vector result;

	result._array[0] = _array[0] + vec._array[0];
	result._array[1] = _array[1] + vec._array[1];
	result._array[2] = _array[2] + vec._array[2];

	return result;
}


Vector& Vector::operator+=(const Vector& vec)
{
	_array[0] += vec._array[0];
	_array[1] += vec._array[1];
	_array[2] += vec._array[2];

	return *this;
}


Vector Vector::operator-(const Vector& vec) const
{
	Vector result;

	result._array[0] = _array[0] - vec._array[0];
	result._array[1] = _array[1] - vec._array[1];
	result._array[2] = _array[2] - vec._array[2];

	return result;
}


Vector Vector::operator-() const
{
	Vector result;

	result._array[0] = -_array[0];
	result._array[1] = -_array[1];
	result._array[2] = -_array[2];

	return result;
}


Vector& Vector::operator-=(const Vector& vec)
{
	_array[0] -= vec._array[0];
	_array[1] -= vec._array[1];
	_array[2] -= vec._array[2];

	return *this;
}


float Vector::operator*(const Vector& vec) const
{
	float result = 0.0f;

	result += _array[0] * vec._array[0];
	result += _array[1] * vec._array[1];
	result += _array[2] * vec._array[2];

	return result;
}


Vector Vector::operator%(const Vector& vec) const
{
	Vector result;

	result._array[0] = _array[1] * vec._array[2] - _array[2] * vec._array[1];
	result._array[1] = _array[2] * vec._array[0] - _array[0] * vec._array[2];
	result._array[2] = _array[0] * vec._array[1] - _array[1] * vec._array[0];

	return result;
}


Vector Vector::operator*(float c) const
{
	Vector result;

	result._array[0] = c * _array[0];
	result._array[1] = c * _array[1];
	result._array[2] = c * _array[2];

	return result;
}


void print(Vector& vec)
{
	cout << vec._array[0] << "  " << vec._array[1] << "  " << vec._array[2];
}
