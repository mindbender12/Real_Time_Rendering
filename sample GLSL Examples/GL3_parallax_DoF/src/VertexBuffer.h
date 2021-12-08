#ifndef CLASS_VERTEX_BUFFER
#define CLASS_VERTEX_BUFFER

#include <stdexcept>
#include <GL/gl.h>
#include <GL/glext.h>

/// Vertex Buffer Object class
class VertexBuffer
{
	protected:
		GLuint _vert_id;
		unsigned long int _vert_len;
		GLuint _norm_id;
		unsigned long int _norm_len;
		GLuint _tex_id;
		unsigned long int _tex_len;
		GLuint _tan_id;
		unsigned long int _tan_len;
		GLuint _ind_id;
		unsigned long int _ind_len;
	public:
		/// Default Constructor
		VertexBuffer() : _vert_id(0), _norm_id(0), _tex_id(0), _tan_id(0), _ind_id(0),
		                 _vert_len(0), _norm_len(0), _tex_len(0), _tan_len(0), _ind_len(0)
		                 { };
		/// Destructor
		~VertexBuffer();
		/// Set vertex data
		void SetVertices(GLsizeiptr count, GLvoid *data);
		/// Set normal data
		void SetNormals(GLsizeiptr count, GLvoid *data);
		/// Set texcoords data
		void SetTexCoords(GLsizeiptr count, GLvoid *data);
		/// Set tangents data
		void SetTangents(GLsizeiptr count, GLvoid *data);
		/// Set index data
		void SetIndices(GLsizeiptr count, GLvoid *data);
		/// Bind the VBO to the context
		void Bind();
		/// Draw the VBO via glDrawArray()
		void DrawArray(GLsizei count = 0) throw(std::out_of_range);
		/// Draw the VBO via glDrawElements()
		void DrawElements(GLsizei count = 0) throw(std::out_of_range);
};

#endif

