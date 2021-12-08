#ifndef CLASS_MESH
#define CLASS_MESH

#include <stdexcept>
#include <string>
#include "VertexBuffer.h"

/// Basic mesh class
class Mesh
{
	protected:
		unsigned long int _nvert;
		unsigned long int _ntri;
		GLfloat *_vertices;
		GLfloat *_normals;
		GLfloat *_texcoords;
		GLuint *_indices;
		GLfloat *_tangents;
		VertexBuffer _vbo;

		/// Load a Stanford PLY file
		void LoadPLY(const char *filename) throw(std::domain_error);
		/// Setup the VBOs
		void SetupVBO();
		/// Calculate face tangents
		void CalcTangents();
		void ClassTangents();
	public:
		/// Default constructor
		Mesh(const std::string& filename) throw(std::invalid_argument);
		/// Draw the mesh
		void Draw();
};

#endif

