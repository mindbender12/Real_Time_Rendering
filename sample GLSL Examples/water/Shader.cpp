// Copyright Jay Conrod 2010.
// http://jayconrod.com
// jayconrod@gmail.com
//
// You are free to modify and distribute this code as long as this
// copyright notice is maintained.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

#include "Shader.h"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <GL/gl.h>
#include "Utilities.h"

using namespace std;
using namespace boost;

enum UniformType {
    INT,
    FLOAT1,
    FLOAT2,
    FLOAT3,
    FLOAT4
};

struct Uniform {
    UniformType type;
    union {
        int i;
        float f[4];
    } value;
};

Shader::Shader(const string &vertexSource, const string &fragmentSource) {
    program = glCreateProgram();

    compileAndAttach(GL_VERTEX_SHADER, &vertexShader, vertexSource);
    compileAndAttach(GL_FRAGMENT_SHADER, &fragmentShader, fragmentSource);

    glLinkProgram(program);
}

Shader::~Shader() {
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);
}

void Shader::bind() {
    glUseProgram(program);
    for (map<string, Uniform>::iterator i = uniforms.begin(); i != uniforms.end(); ++i) {
        string name = i->first;
        Uniform uniform = i->second;
        GLint location = glGetUniformLocation(program, name.c_str());
        assert(location >= 0);
        if (uniform.type == INT) 
            glUniform1i(location, uniform.value.i);
        if (uniform.type == FLOAT1)
            glUniform1f(location, uniform.value.f[0]);
        else if (uniform.type == FLOAT2)
            glUniform2f(location, uniform.value.f[0], uniform.value.f[1]);
        else if (uniform.type == FLOAT3)
            glUniform3f(location, uniform.value.f[0], uniform.value.f[1], uniform.value.f[2]);
        else if (uniform.type == FLOAT4)
            glUniform4f(location, uniform.value.f[0], uniform.value.f[1], uniform.value.f[2], uniform.value.f[3]);
    }
}

bool Shader::hasUniform(const string &name) {
    glUseProgram(program);
    GLint uniform = glGetUniformLocation(program, name.c_str());
    glUseProgram(0);
    return uniform >= 0;
}

void Shader::setUniform(const string &name, int v1) {
    Uniform uniform;
    uniform.type = INT;
    uniform.value.i = v1;
    uniforms[name] = uniform;
}

void Shader::setUniform(const string &name, float v1) {
    Uniform uniform;
    uniform.type = FLOAT1;
    uniform.value.f[0] = v1;
    uniforms[name] = uniform;
}

void Shader::setUniform(const string &name, float v1, float v2) {
    Uniform uniform;
    uniform.type = FLOAT2;
    uniform.value.f[0] = v1;
    uniform.value.f[1] = v2;
    uniforms[name] = uniform;
}

void Shader::setUniform(const string &name, float v1, float v2, float v3) {
    Uniform uniform;
    uniform.type = FLOAT3;
    uniform.value.f[0] = v1;
    uniform.value.f[1] = v2;
    uniform.value.f[2] = v3;
    uniforms[name] = uniform;
}

void Shader::setUniform(const string &name, float v1, float v2, float v3, float v4) {
    Uniform uniform;
    uniform.type = FLOAT4;
    uniform.value.f[0] = v1;
    uniform.value.f[1] = v2;
    uniform.value.f[2] = v3;
    uniform.value.f[3] = v4;
    uniforms[name] = uniform;
}

shared_ptr<Shader> Shader::fromFiles(const string &vertexFilename,
                                     const string &fragmentFilename)
{
    string vertexSource = readTextFromFile(vertexFilename);
    string fragmentSource = readTextFromFile(fragmentFilename);
    shared_ptr<Shader> shader(new Shader(vertexSource, fragmentSource));
    return shader;
}

void Shader::compileAndAttach(GLenum which, GLuint *shader, const string &source) {
    *shader = glCreateShader(which);
    const GLchar *buffer = source.c_str();
    GLsizei length = source.length();
    glShaderSource(*shader, 1, &buffer, &length);
    glCompileShader(*shader);
    GLint status;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *buffer = new GLchar[logLength];
        glGetShaderInfoLog(*shader, logLength, &logLength, buffer);
        cerr << buffer << endl;
        delete [] buffer;
    }
    assert(status == GL_TRUE);
    glAttachShader(program, *shader);
}
