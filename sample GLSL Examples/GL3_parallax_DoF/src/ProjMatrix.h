#ifndef CLASS_PROJMATRIX
#define CLASS_PROJMATRIX

#include "Matrix.h"

/// Matrix class supporting common projection transformations
class ProjMatrix : public Matrix
{
	public:
		ProjMatrix(int depth = 0) : Matrix(depth) { };
		void Frustum(float left, float right, float bottom, float top, float near_val, float far_val);
		void Ortho(float left, float right, float bottom, float top, float near_val, float far_val);
		void Perspective(float fovY, float aspect, float zNear, float zFar);
};

#endif

