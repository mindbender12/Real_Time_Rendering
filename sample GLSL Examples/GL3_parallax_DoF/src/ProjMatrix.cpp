#include <cmath>
#include "ProjMatrix.h"


void ProjMatrix::Frustum(float left, float right, float bottom, float top, float near_val, float far_val)
{
	//FIXME: no checks of parameters validity
	
	float matrix[16] = {0, 0, 0, 0,
	                    0, 0, 0, 0,
	                    0, 0, 0, 0,
	                    0, 0, 0, 0};

	matrix[0]  = 2 * near_val / (right - left);
	matrix[5]  = 2 * near_val / (top - bottom);
	matrix[8]  = (right + left) / (right - left);
	matrix[9]  = (top + bottom) / (top - bottom);
	matrix[10] = -(far_val + near_val) / (far_val - near_val);
	matrix[11] = -1;
	matrix[14] = -2 * far_val * near_val / (far_val - near_val);

	RightMult(matrix);
}


void ProjMatrix::Ortho(float left, float right, float bottom, float top, float near_val, float far_val)
{
	//FIXME: no checks on parameters validity
	
	float matrix[16] = {0, 0, 0, 0,
	                    0, 0, 0, 0,
	                    0, 0, 0, 0,
	                    0, 0, 0, 0};

	matrix[0]  = 2 / (right - left);
	matrix[5]  = 2 / (top - bottom);
	matrix[10] =-2 / (far_val - near_val);
	matrix[12] = -(right + left) / (right - left);
	matrix[13] = -(top + bottom) / (top - bottom);
	matrix[14] = -(far_val + near_val) / (far_val - near_val);
	matrix[15] = 1;

	RightMult(matrix);
}


void ProjMatrix::Perspective(float fovY, float aspect, float zNear, float zFar)
{
	float fW, fH;

	fH = tan((fovY/2) / 180 * 3.14159265358979323846 ) * zNear;
	fW = fH * aspect;

	Frustum(-fW, fW, -fH, fH, zNear, zFar);
}

