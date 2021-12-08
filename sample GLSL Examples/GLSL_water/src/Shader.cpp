#include <iostream>
#include <fstream>
#include <stdexcept>

#include <GL/gl.h>
#include <GL/glext.h>

#include "Shader.h"

using std::ios;
using std::ifstream;
using std::invalid_argument;

using std::cout;
using std::endl;

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


void Shader::Load(const char *filename) throw(invalid_argument)
{
	int length;
	char *buffer;

	ifstream is;
	is.open(filename, ios::in);
	std::cout << "Loading " << filename << std::endl;
	if (is.fail()) {
		throw invalid_argument("Shader::Load - cannot load the file");
	}

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


void Shader::PrintInfoLog()
{
	int length = 0;
	char *infoLog;

	glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);
	if (length > 1) {
		infoLog = new char[length];
		glGetShaderInfoLog(_id, length, NULL, infoLog);
		cout << infoLog;
		delete[] infoLog;
	}
}
