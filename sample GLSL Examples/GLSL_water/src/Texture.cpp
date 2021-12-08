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


Texture::Texture(string filename) : _id(0)
{
	glGenTextures(1, &_id);
	Bind();
	Load(filename);
}


Texture::Texture(SDL_Surface *image) : _id(0)
{
	glGenTextures(1, &_id);
	Bind();
	Load(image);
}


Texture::Texture(GLenum iformat, int width, int height, GLfloat *pixels) : _id(0)
{
	glGenTextures(1, &_id);
	Bind();
	Load(iformat, width, height, pixels);
}


Texture::Texture(GLenum iformat, int width, int height, GLubyte *pixels) : _id(0)
{
	glGenTextures(1, &_id);
	Bind();
	Load(iformat, width, height, pixels);
}


Texture::~Texture()
{
	glDeleteTextures(1, &_id);
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


void Texture::Load(SDL_Surface *surface) throw(invalid_argument)
{
	Uint8 bpp;
	GLenum iformat, format;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	bpp = surface->format->BitsPerPixel;
	if (bpp == 32) {
		iformat = GL_RGBA8;
		if (surface->format->Rmask == 0x000000ff ) {
			format = GL_RGBA;
		}
		else {
			format = GL_BGRA;
		}
	}
	else if (bpp  == 24) {
		iformat = GL_RGB8;
		if (surface->format->Rmask == 0x000000ff) {
			format = GL_RGB;
		}
		else {
			format = GL_BGR;
		}
	}
	else {
		throw invalid_argument("Texture::Load - Not a truecolor image");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, iformat, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
}


void Texture::Load(GLenum iformat, int width, int height, GLfloat *pixels)
{
	GLenum format;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (iformat == GL_RGB16F_ARB || iformat == GL_RGB32F_ARB) {
		format = GL_RGB;
	}
	else if (iformat == GL_RGBA16F_ARB || iformat == GL_RGB32F_ARB) {
		format = GL_RGBA;
	}
	else if (iformat == GL_RGBA8 || iformat == GL_RGBA || iformat == 4) {
		format = GL_RGBA;
	}
	else if (iformat == GL_RGB8 || iformat == GL_RGB || iformat == 3) {
		format = GL_RGB;
	}
	else if (iformat == GL_LUMINANCE8_ALPHA8 || iformat == GL_LUMINANCE_ALPHA || iformat == 2) {
		format = GL_LUMINANCE_ALPHA;
	}
	else if (iformat == GL_LUMINANCE8 || iformat == GL_LUMINANCE || iformat == 1) {
		format = GL_LUMINANCE;
	}
	else if (iformat == GL_DEPTH_COMPONENT || iformat == GL_DEPTH_COMPONENT16_ARB || 
		iformat == GL_DEPTH_COMPONENT24_ARB || iformat == GL_DEPTH_COMPONENT32_ARB) {
		format = GL_DEPTH_COMPONENT;
	}
	else {
		throw invalid_argument("Texture::Load - Unknown internal format");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_FLOAT, pixels);
}


void Texture::Load(GLenum iformat, int width, int height, GLubyte *pixels)
{
	GLenum format;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if (iformat == GL_RGB8) {
		format = GL_RGB;
	}
	else if (iformat == GL_RGBA8) {
		format = GL_RGBA;
	}
	else {
		throw invalid_argument("Texture::Load - Unknown internal format");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
}


void Texture::SetFiltering(GLenum filter)
{
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}
