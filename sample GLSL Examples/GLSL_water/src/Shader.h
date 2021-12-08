#ifndef CLASS_SHADER
#define CLASS_SHADER

#include <stdexcept>

/// Shader class
class Shader
{
	private:
		GLuint _id;
	public:
		Shader(GLenum type);
		~Shader();
		void Compile();
		void Load(const char *filename) throw(std::invalid_argument);
		void Attach(GLuint program);
		void PrintInfoLog();
};

#endif

