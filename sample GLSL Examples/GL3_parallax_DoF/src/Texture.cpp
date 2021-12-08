#include <iostream>
#include <string>
#include <stdexcept>

#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "Texture.h"

using std::string;
using std::invalid_argument;


Texture::Texture() : _id(0)
{
	glGenTextures(1, &_id);
}


Texture::Texture(string filename, int unit) : _id(0)
{
	glGenTextures(1, &_id);
	Bind(unit);
	Load(filename);
}


Texture::Texture(SDL_Surface *image, int unit) : _id(0)
{
	glGenTextures(1, &_id);
	Bind(unit);
	Load(image);
}


Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}


void Texture::Bind(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, _id);
}


void Texture::Load(string filename) throw(invalid_argument)
{
	SDL_Surface *image;

	std::cout << "Loading " << filename << std::endl;

	if (!(image = (SDL_Surface *)IMG_Load(filename.c_str()))) {
		throw invalid_argument("Texture::Load - loading error");
	}

	Load(image);

	SDL_FreeSurface(image);
}


void Texture::Load(SDL_Surface *image)
{
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if ((image->format)->Amask) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	}
}


void Texture::Unbind(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
