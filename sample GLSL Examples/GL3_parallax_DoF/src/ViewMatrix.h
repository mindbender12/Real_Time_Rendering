#ifndef CLASS_VIEWMATRIX
#define CLASS_VIEWMATRIX

#include "Matrix.h"

/// Matrix class supporting common model-view transformations
class ViewMatrix : public Matrix
{
	public:
		ViewMatrix(int depth = 0) : Matrix(depth) { };
		void Translate(float x, float y, float z);
		void Rotate(float angle, float x, float y, float z);
		void Scale(float x, float y, float z);
};

#endif

