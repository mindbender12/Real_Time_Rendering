#ifndef CLASS_SHADER
#define CLASS_SHADER


/// Shader class
class Shader
{
	private:
		GLuint _id;
	public:
		Shader(GLenum type);
		~Shader();
		void Compile();
		void Load(const char *filename);
		void Attach(GLuint program);
};

#endif

