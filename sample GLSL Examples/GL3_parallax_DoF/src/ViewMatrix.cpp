#include <cmath>
#include "ViewMatrix.h"


void ViewMatrix::Translate(float x, float y, float z)
{
	_array[12] += x;
	_array[13] += y;
	_array[14] += z;
}


void ViewMatrix::Rotate(float angle, float x, float y, float z)
{
	// FIXME: no automtic normalization of the vector (x, y, z)

	double radians = angle * 3.14159265358979323846 / 180;
	float rotation[16];

	rotation[0]  = x * x * (1 - cos(radians)) + cos(radians);
	rotation[1]  = y * x * (1 - cos(radians)) + z * sin(radians);
	rotation[2]  = x * z * (1 - cos(radians)) - y * sin(radians);
	rotation[3]  = 0;
	rotation[4]  = x * y * (1 - cos(radians)) - z * sin(radians);
	rotation[5]  = y * y * (1 - cos(radians)) + cos(radians);
	rotation[6]  = y * z * (1 - cos(radians)) + x * sin(radians);
	rotation[7]  = 0;
	rotation[8]  = x * z * (1 - cos(radians)) + y * sin(radians);
	rotation[9]  = y * z * (1 - cos(radians)) - x * sin(radians);
	rotation[10] = z * z * (1 - cos(radians)) + cos(radians);
	rotation[11] = rotation[12] = rotation[13] = rotation[14] = 0;
	rotation[15] = 1;

	RightMult(rotation);
}


void ViewMatrix::Scale(float x, float y, float z)
{
	int i;

	for (i = 0; i < 3; i++) {
		_array[i]   *= x;
		_array[4+i] *= y;
		_array[8+i] *= z;
	}
}
