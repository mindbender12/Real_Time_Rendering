#include <iostream>
#include "Matrix.h"

using std::cout;
using std::endl;

Matrix::Matrix(int depth) : _stack(0), _depth(depth), _top(0)
{
	LoadIdentity();

	if (_depth > 0) {
		_stack = new float[16*_depth];
	}
}


Matrix::~Matrix()
{
	if (_depth > 0) {
		delete[] _stack;
	}
}


void Matrix::SetNull()
{
	int i;

	for(i = 0; i < 16; i++) {
		_array[i] = 0.0;
	}
}


void Matrix::LoadIdentity()
{
	SetNull();

	_array[0]  = 1;
	_array[5]  = 1;
	_array[10] = 1;
	_array[15] = 1;
}


void Matrix::Load(const float *M)
{
	int i;

	for(i=0; i < 16; i++) {
		_array[i] = M[i];
	}	
}


void Matrix::Load(float m11, float m21, float m31, float m41,
                  float m12, float m22, float m32, float m42,
                  float m13, float m23, float m33, float m43,
                  float m14, float m24, float m34, float m44)
{
	_array[0]  = m11; _array[1]  = m21; _array[2]  = m31; _array[3]  = m41;
	_array[4]  = m12; _array[5]  = m22; _array[6]  = m32; _array[7]  = m42;
	_array[8]  = m13; _array[9]  = m23; _array[10] = m33; _array[11] = m43;
	_array[12] = m14; _array[13] = m24; _array[14] = m34; _array[15] = m44;

}


void Matrix::Invert()
{
	float det;
	float result[16];

	det =  _array[0] * (_array[5] * _array[10] - _array[6] * _array[9]) -
	       _array[1] * (_array[4] * _array[10] - _array[6] * _array[8]) +
	       _array[2] * (_array[4] * _array[9] - _array[5] * _array[8]);

	if (det == 0.0) return;

	det = 1.0 / det;

	result[0] = det * (_array[5] * _array[10] - _array[6] * _array[9]);
	result[1] =-det * (_array[1] * _array[10] - _array[2] * _array[9]);
	result[2] = det * (_array[1] * _array[6] - _array[2] * _array[5]);
	result[3] = 0.0;

	result[4] =-det * (_array[4] * _array[10] - _array[6] * _array[8]);
	result[5] = det * (_array[0] * _array[10] - _array[2] * _array[8]);
	result[6] =-det * (_array[0] *  _array[6] - _array[2] * _array[4]);
	result[7] = 0.0;

	result[8] = det * (_array[4] * _array[9] - _array[5] * _array[8]);
	result[9] =-det * (_array[0] * _array[9]- _array[1] * _array[8]);
	result[10] = det * (_array[0] * _array[5] - _array[1] * _array[4]);
	result[11] = 0.0;

	result[12] = -(_array[12] * result[0] + _array[13] * result[4] + _array[14] * result[8]);
	result[13] = -(_array[12] * result[1] + _array[13] * result[5] + _array[14] * result[9]);
	result[14] = -(_array[12] * result[2] + _array[13] * result[6] + _array[14] * result[10]);
	result[15] = 1.0;

	Load(result);
}


void Matrix::Transpose()
{
	float x;

	x = _array[1];  _array[1]  = _array[4];  _array[4]  = x;
	x = _array[2];  _array[2]  = _array[8];  _array[8]  = x;
	x = _array[3];  _array[3]  = _array[12]; _array[12] = x;
	x = _array[6];  _array[6]  = _array[9];  _array[9]  = x;
	x = _array[7];  _array[7]  = _array[13]; _array[13] = x;
	x = _array[11]; _array[11] = _array[14]; _array[14] = x;
}


float *Matrix::VecMult(float *V) const
{
	int i, j;
	float vec[4];

	for(i = 0; i < 4; i++) {
		vec[i] = V[i];
	}

	for(i = 0; i < 4; i++) {
		V[i] = 0;
		for(j = 0; j < 4; j++) {
			V[i] += _array[j*4 + i] * vec[j];
		}
	}

	return V;
}


Matrix& Matrix::RightMult(float *M)
{
	float result[16] = {0, 0, 0, 0,
	                    0, 0, 0, 0,
	                    0, 0, 0, 0,
	                    0, 0, 0, 0};
	int i, j, k;

 	 for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				result[i*4 + j] += _array[k*4 + j] * M[i*4 + k];
			}
		}
	}

	Load(result);

	return *this;
}


void Matrix::Push()
{
	int i;

	if (_top >= _depth) {
		return;
	}

	for(i = 0; i < 16; i++) {
		_stack[16 * _top + i] = _array[i];
	}

	_top++;
}


void Matrix::Pop()
{
	if (_top < 1) {
		return;
	}

	_top--;

	Load(&_stack[16 * _top]);
}


Matrix Matrix::operator*(const Matrix& mat) const
{
	Matrix result;
	int i, j, k;

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				result._array[i*4 + j] += _array[i*4 + k] * mat._array[k*4 + j];
			}
		}
	}

	return result;
}


void print(Matrix& mat)
{
	int i, j;

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			cout << mat._array[4*i+j] << "  ";
		}
		cout << endl;
	}
}



void print_transposed(Matrix& mat)
{
	int i, j;

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			cout << mat._array[i+4*j] << "  ";
		}
		cout << endl;
	}
}
