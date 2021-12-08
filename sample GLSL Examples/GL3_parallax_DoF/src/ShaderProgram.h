#ifndef CLASS_SHADERPROGRAM
#define CLASS_SHADERPROGRAM

#include <vector>
#include "Shader.h"

/// ShaderProgram class
class ShaderProgram
{
	private:
		GLuint _id;
		std::vector<Shader *> _shaders;
	public:
		ShaderProgram();
		ShaderProgram(const char *vertex, const char *fragment);
		~ShaderProgram();
		void AttachShader(Shader *sh);
		void Link();
		void Use();
		void Uniform(const char* name, GLint value);
		void Uniform(const char* name, GLfloat value);
		void Uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void Uniform4v(const char* name, GLfloat *array);
		void UniformMatrix4(const char* name, const GLfloat *array);
		void PrintInfoLog();
};

#endif

