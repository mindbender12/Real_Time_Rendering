#ifndef CLASS_TEXTURE
#define CLASS_TEXTURE

#include <stdexcept>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>

/// Texture class
class Texture
{
	private:
		GLuint _id;
		void Load(std::string filename) throw(std::invalid_argument);
		void Load(SDL_Surface *image) throw(std::invalid_argument);
		void Load(GLenum iformat, int width, int height, GLfloat *pixels);
		void Load(GLenum iformat, int width, int height, GLubyte *pixels);
	public:
		Texture();
		Texture(std::string filename);
		Texture(SDL_Surface *surface);
		Texture(GLenum iformat, int width, int height, GLfloat *pixels);
		Texture(GLenum iformat, int width, int height, GLubyte *pixels);
		~Texture();
		/// Set texture filtering for both magnification and minification
		void SetFiltering(GLenum filter);
		/// Bind the texture to the current unit
		void Bind() { glBindTexture(GL_TEXTURE_2D, _id); };
		/// Disable texture rendering for the current unit
		static void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); };
};

#endif
