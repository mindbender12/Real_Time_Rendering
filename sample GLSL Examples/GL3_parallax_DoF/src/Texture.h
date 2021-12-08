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
		void Load(SDL_Surface *image);
	public:
		Texture();
		Texture(std::string filename, int unit = 0);
		Texture(SDL_Surface *image, int unit = 0);
		~Texture();
		/// Bind the texture to a specific texture unit
		void Bind(int unit = 0);
		/// Disable texture rendering of a specific unit
		static void Unbind(int unit = 0);
};

#endif

