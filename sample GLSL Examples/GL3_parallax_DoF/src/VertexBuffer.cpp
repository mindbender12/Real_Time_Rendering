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


void VertexBuffer::SetVertices(GLsizeiptr count, GLvoid *data)
{
	if (glIsBuffer(_vert_id)) {
		glDeleteBuffers(1, &_vert_id);
	}

	glGenBuffers(1, &_vert_id);
	glBindBuffer(GL_ARRAY_BUFFER, _vert_id);
	glBufferData(GL_ARRAY_BUFFER, (count*3)*sizeof(GLfloat), data, GL_STATIC_DRAW);

	_vert_len = count;
}


void VertexBuffer::SetNormals(GLsizeiptr count, GLvoid *data)
{
	if (glIsBuffer(_norm_id)) {
		glDeleteBuffers(1, &_norm_id);
	}

	glGenBuffers(1, &_norm_id);
	glBindBuffer(GL_ARRAY_BUFFER, _norm_id);
	glBufferData(GL_ARRAY_BUFFER, (count*3)*sizeof(GLfloat), data, GL_STATIC_DRAW);

	_norm_len = count;
}


void VertexBuffer::SetTexCoords(GLsizeiptr count, GLvoid *data)
{
	if (glIsBuffer(_tex_id)) {
		glDeleteBuffers(1, &_tex_id);
	}

	glGenBuffers(1, &_tex_id);
	glBindBuffer(GL_ARRAY_BUFFER, _tex_id);
	glBufferData(GL_ARRAY_BUFFER, (count*2)*sizeof(GLfloat), data, GL_STATIC_DRAW);

	_tex_len = count;
}


void VertexBuffer::SetTangents(GLsizeiptr count, GLvoid *data)
{
	if (glIsBuffer(_tan_id)) {
		glDeleteBuffers(1, &_tan_id);
	}

	glGenBuffers(1, &_tan_id);
	glBindBuffer(GL_ARRAY_BUFFER, _tan_id);
	glBufferData(GL_ARRAY_BUFFER, (count*3)*sizeof(GLfloat), data, GL_STATIC_DRAW);

	_tan_len = count;
}


void VertexBuffer::SetIndices(GLsizeiptr count, GLvoid *data)
{
	if (glIsBuffer(_ind_id)) {
		glDeleteBuffers(1, &_ind_id);
	}

	glGenBuffers(1, &_ind_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ind_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(GLuint), data, GL_STATIC_DRAW);

	_ind_len = count;
}


void VertexBuffer::Bind()
{
	GLint prog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

	glBindBuffer(GL_ARRAY_BUFFER, _vert_id);
	glBindAttribLocation(prog, 0, "Vertex");
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, NULL);
	glEnableVertexAttribArray(0);

	if (_norm_len > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _norm_id);
		glBindAttribLocation(prog, 1, "Normal");
		glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, NULL);
		glEnableVertexAttribArray(1);
	}

	if (_tex_len > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _tex_id);
		glBindAttribLocation(prog, 2, "TexCoord");
		glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, NULL);
		glEnableVertexAttribArray(2);
	}

	if (_tan_len > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _tan_id);
		glBindAttribLocation(prog, 3, "Tangent");
		glVertexAttribPointer(3, 3, GL_FLOAT, false, 0, NULL);
		glEnableVertexAttribArray(3);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ind_id);
}


void VertexBuffer::DrawArray(GLsizei count) throw(out_of_range)
{
	if (count == 0) {
		count = _vert_len;
	}
	else if (count > _vert_len) {
		throw out_of_range("VertexBuffer::DrawArrays - number of vertices exceded");
	}

	Bind();

	glDrawArrays(GL_TRIANGLES, 0, count);
}


void VertexBuffer::DrawElements(GLsizei count) throw(out_of_range)
{
	if (count == 0) {
		count = _ind_len;
	}
	else if (count > _ind_len) {
		throw out_of_range("VertexBuffer::DrawElements - number of indices exceded");
	}

	Bind();

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}
