#include <iostream>
#include <fstream>

#include <GL/gl.h>
#include <GL/glext.h>

#include "Shader.h"

using std::ios;
using std::ifstream;

Shader::Shader(GLenum type)
{
	_id = glCreateShader(type);
}


Shader::~Shader()
{
	glDeleteShader(_id);
}


void Shader::Compile()
{
	glCompileShader(_id);
}


void Shader::Load(const char *filename)
{
	int length;
	char *buffer;

	ifstream is;
	is.open(filename, ios::in);
	std::cout << "Loading " << filename << std::endl;

	// get length of file
	is.seekg(0, ios::end);
	length = is.tellg();
	is.seekg(0, ios::beg);

	// allocate memory:
	buffer = new char[length+1];

	// read data as a block:
	is.read(buffer, length);
	is.close();

	buffer[length] = '\0';
	const char *source = buffer;
	glShaderSource(_id, 1, &source, NULL);

	delete[] buffer;
}


void Shader::Attach(GLuint program)
{
	glAttachShader(program, _id);
}
