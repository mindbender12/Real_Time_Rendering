#ifndef CLASS_MESH_LOADER
#define CLASS_MESH_LOADER

#include <stdexcept>
#include <string>
#include "VertexBuffer.h"

/// Mesh loading class
class MeshLoader
{
	protected:
		long int _nvert;
		long int _ntri;
		GLfloat *_vertices;
		GLfloat *_normals;
		GLfloat *_texcoords;
		GLfloat *_tangents;
		GLuint *_indices;
		VertexBuffer *_vbo;

		/// Load a Stanford PLY file
		void LoadPLY(const char *filename) throw(std::domain_error);
		/// Setup the VBOs
		void SetupVBO();
		/// Calculate face tangents
		void CalcTangents();
	public:
		/// Default Constructor
		MeshLoader() : _nvert(0), _ntri(0), _vertices(NULL), _normals(NULL), _texcoords(NULL), _tangents(NULL), _indices(NULL), _vbo(NULL) {};
		/// VertexBuffer builder
		VertexBuffer *Load(const std::string& filename, VertexBuffer *vbo = NULL) throw(std::invalid_argument);
};

#endif

