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
		GLuint _norm_id;
		GLuint _tex_id;
		GLuint _tan_id;
		GLuint _ind_id;

		GLsizei _vert_len;
		GLsizei _norm_len;
		GLsizei _tex_len;
		GLsizei _tan_len;
		GLsizei _ind_len;

		bool _do_binding;
	public:
		/// Default Constructor
		VertexBuffer() : _vert_id(0), _norm_id(0), _tex_id(0), _tan_id(0), _ind_id(0),
		                 _vert_len(0), _norm_len(0), _tex_len(0), _tan_len(0), _ind_len(0),
						 _do_binding(true)
		                 { };
		/// Destructor
		~VertexBuffer();
		/// Set vertex data
		void SetVertices(GLsizeiptr count, GLvoid *data, GLenum usage = GL_STATIC_DRAW);
		/// Set normal data
		void SetNormals(GLsizeiptr count, GLvoid *data, GLenum usage = GL_STATIC_DRAW);
		/// Set texcoord data
		void SetTexCoords(GLsizeiptr count, GLvoid *data, GLenum usage = GL_STATIC_DRAW);
		/// Set tangents data
		void SetTangents(GLsizeiptr count, GLvoid *data, GLenum usage = GL_STATIC_DRAW);
		/// Set index data
		void SetIndices(GLsizeiptr count, GLvoid *data, GLenum usage = GL_STATIC_DRAW);
		/// Choose whether to bind the VBO or not prior to a drawing request
		void SetBinding(bool do_binding);
		/// Bind the VBO to the context
		void Bind();
		/// Draw the VBO via glDrawArrays() from the start
		void DrawArrays(GLsizei count = 0) throw(std::out_of_range);
		/// Draw the VBO via glDrawArrays starting at a precise index
		void DrawArrays(GLint first, GLsizei count) throw(std::out_of_range);
		/// Draw the VBO via glDrawElements()
		void DrawElements(GLsizei count = 0) throw(std::out_of_range);
		/// Draw the VBO via GLDrawElements starting at a precise index
		void DrawElements(GLsizei offset, GLsizei count) throw(std::out_of_range);
};

#endif

