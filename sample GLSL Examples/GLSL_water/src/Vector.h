#ifndef CLASS_VECTOR
#define CLASS_VECTOR

#include <cstring>

/// Generic vector class
class Vector
{
	protected:
		float _array[3];
	public:
		/// Default constructor, all zeros
		Vector();
		/// Construct from components
		Vector(float v0, float v1, float v2);
		/// Construct from pointer
		Vector(float *vec);

		// Property methods
		float X() {return _array[0];}
		void X(float x ) {_array[0] = x;}
		float Y() {return _array[1];}
		void Y(float y ) {_array[1] = y;}
		float Z() {return _array[2];}
		void Z(float z ) {_array[2] = z;}

		/// Set null every element of the vector
		void SetNull() { memset(_array, 0, sizeof(float) * 3); };
		/// Return the vector as a C array
		const float* c_array() const {return _array;};
		/// Load a vector from single elements
		void Load(float v0, float v1, float v2);
		/// Load a vector from an array
		void Load(float *V);
		/// Calculate the length of the vector
		float Length() const;
		/// Normalize the vector
		Vector& Normalize();

		// Operators
		bool operator==(const Vector& vec) const;
		Vector operator+(const Vector& vec) const;
		Vector& operator+=(const Vector& vec);
		Vector operator-(const Vector& vec) const;
		/// Unary negation
		Vector operator-() const;
		Vector& operator-=(const Vector& vec);
		/// Calculate dot product
		float operator*(const Vector& vec) const;
		/// Calculate cross product
		Vector operator%(const Vector& vec) const;
		/// Multiplication by a constant scalar
		Vector operator*(float c) const;

		friend void print(Vector& vec);
};

#endif

