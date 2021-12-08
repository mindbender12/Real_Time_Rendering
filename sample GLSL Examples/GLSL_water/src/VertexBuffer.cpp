#include <stdexcept>

#include <GL/gl.h>
#include <GL/glext.h>

#include "VertexBuffer.h"

using std::out_of_range;

#include <iostream>
using std::cout;
using std::endl;

VertexBuffer::~VertexBuffer()
{
	if (glIsBuffer(_vert_id)) {
		glDeleteBuffers(1, &_vert_id);
	}

	if (glIsBuffer(_norm_id)) {
		glDeleteBuffers(1, &_norm_id);
	}

	if (glIsBuffer(_tex_id)) {
		glDeleteBuffers(1, &_tex_id);
	}

	if (glIsBuffer(_tan_id)) {
		glDeleteBuffers(1, &_tan_id);
	}

	if (glIsBuffer(_ind_id)) {
		glDeleteBuffers(1, &_ind_id);
	}
}


void VertexBuffer::SetVertices(GLsizeiptr count, GLvoid *data, GLenum usage)
{
	if (!glIsBuffer(_vert_id)) {
		glGenBuffers(1, &_vert_id);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vert_id);
	glBufferData(GL_ARRAY_BUFFER, (count*3)*sizeof(GLfloat), data, usage);

	_vert_len = count;
}


void VertexBuffer::SetNormals(GLsizeiptr count, GLvoid *data, GLenum usage)
{
	if (!glIsBuffer(_norm_id)) {
		glGenBuffers(1, &_norm_id);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _norm_id);
	glBufferData(GL_ARRAY_BUFFER, (count*3)*sizeof(GLfloat), data, usage);

	_norm_len = count;
}


void VertexBuffer::SetTexCoords(GLsizeiptr count, GLvoid *data, GLenum usage)
{
	if (!glIsBuffer(_tex_id)) {
		glGenBuffers(1, &_tex_id);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _tex_id);
	glBufferData(GL_ARRAY_BUFFER, (count*2)*sizeof(GLfloat), data, usage);

	_tex_len = count;
}


void VertexBuffer::SetTangents(GLsizeiptr count, GLvoid *data, GLenum usage)
{
	if (!glIsBuffer(_tan_id)) {
		glGenBuffers(1, &_tan_id);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _tan_id);
	glBufferData(GL_ARRAY_BUFFER, (count*4)*sizeof(GLfloat), data, usage);

	_tan_len = count;
}


void VertexBuffer::SetIndices(GLsizeiptr count, GLvoid *data, GLenum usage)
{
	if (!glIsBuffer(_ind_id)) {
		glGenBuffers(1, &_ind_id);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ind_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(GLuint), data, usage);

	_ind_len = count;
}


void VertexBuffer::SetBinding(bool do_binding)
{
	_do_binding = do_binding;
}


void VertexBuffer::Bind()
{
	GLint prog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

	glBindBuffer(GL_ARRAY_BUFFER, _vert_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glEnableClientState(GL_VERTEX_ARRAY);

	if (_norm_len > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _norm_id);
		glNormalPointer(GL_FLOAT, 0, NULL);
		glEnableClientState(GL_NORMAL_ARRAY);
	}
	else
		glDisableClientState(GL_NORMAL_ARRAY);

	if (_tex_len > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _tex_id);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	GLint index;
	index = glGetAttribLocation(prog, "Tangent");
	if (_tan_len > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _tan_id);
		glVertexAttribPointer(index, 4, GL_FLOAT, false, 0, NULL);
		glEnableVertexAttribArray(index);
	}
	else
		glDisableVertexAttribArray(index);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ind_id);
}


void VertexBuffer::DrawArrays(GLsizei count) throw(out_of_range)
{
	if (count == 0) {
		count = _vert_len;
	}
	else if (count > _vert_len) {
		throw out_of_range("VertexBuffer::DrawArrays - number of vertices exceeded");
	}

	if (_do_binding) {
		Bind();
	}

	glDrawArrays(GL_TRIANGLES, 0, count);
}


void VertexBuffer::DrawArrays(GLint first, GLsizei count) throw(out_of_range)
{
//	if (count + first > _vert_len) {
//		throw out_of_range("VertexBuffer::DrawArrays - number of vertices exceeded");
//	}

	if (_do_binding) {
		Bind();
	}

	glDrawArrays(GL_TRIANGLES, first, count);
}


void VertexBuffer::DrawElements(GLsizei count) throw(out_of_range)
{
	if (count == 0) {
		count = _ind_len;
	}
	else if (count > _ind_len) {
		throw out_of_range("VertexBuffer::DrawElements - number of indices exceeded");
	}

	if (_do_binding) {
		Bind();
	}

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}


void VertexBuffer::DrawElements(GLsizei offset, GLsizei count) throw(out_of_range)
{
	if (count == 0) {
		count = _ind_len;
	}
	else if (count > _ind_len) {
		throw out_of_range("VertexBuffer::DrawElements - number of indices exceeded");
	}

	if (_do_binding) {
		Bind();
	}

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, static_cast<GLuint*>(0) + offset);
}
