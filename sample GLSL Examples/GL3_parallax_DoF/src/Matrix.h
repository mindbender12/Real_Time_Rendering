#ifndef CLASS_MATRIX
#define CLASS_MATRIX

/// Generic matrix class (with a stack system)
class Matrix
{
	protected:
		float _array[16];
		float *_stack;
		int _depth;
		int _top;
	public:
		/// Default constructor, all zeros
		Matrix(int depth = 0);
		/// Destructor
		~Matrix();
		/// Set null every element of the matrix
		void SetNull();
		/// Load the identity matrix
		void LoadIdentity();
		/// Return the matrix as a column-major ordered C array
		const float* c_array() const {return _array;};
		/// Load a matrix from an array (column-major order)
		void Load(const float *M);
		/// Load a matrix from single elements (column-major order)
		void Load(float m11, float m21, float m31, float m41,
		          float m12, float m22, float m32, float m42,
		          float m13, float m23, float m33, float m43,
		          float m14, float m24, float m34, float m44);
		/// Transpose the matrix
		void Transpose();
		/// Invert the matrix
		void Invert();
		/// Vector multiplication (this * (v0, v1, v2, v3))
		float* VecMult(float *V) const;
		/// Matrix right multiplication (this * M)
		Matrix& RightMult(float *M);
		/// Push the current matrix in the stack
		void Push();
		/// Pop a matrix from the stack
		void Pop();

		Matrix operator*(const Matrix& mat) const;

		friend void print(Matrix& mat);
		friend void print_transposed(Matrix& mat);
};

#endif

