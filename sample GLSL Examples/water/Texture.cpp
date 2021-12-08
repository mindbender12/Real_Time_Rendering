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

#include "Texture.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <cassert>
#include <cstring>
#include <CImg.h>
#include <boost/shared_ptr.hpp>
#include "Utilities.h"

using namespace std;
using namespace boost;
using namespace cimg_library;

Texture::Texture(int width, int height, int channels, shared_array<unsigned char> data) :
    width(width),
    height(height),
    channels(channels),
    data(data),
    id(0)
{
    assert(channels == 3 || channels == 4);
    assert(isPowerOfTwo(width) && isPowerOfTwo(height));
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D,
                      channels == 3 ? GL_RGB8 : GL_RGBA8,
                      width,
                      height,
                      channels == 3 ? GL_RGB : GL_RGBA,
                      GL_UNSIGNED_BYTE,
                      data.get());
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

GLuint Texture::getId() const {
    return id;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}

int Texture::getChannels() const {
    return channels;
}

shared_array<unsigned char> Texture::getData() const {
    return data;
}

shared_ptr<Texture> Texture::fromFile(const string &filename) {
    CImg<unsigned char> image(filename.c_str());
    int width = image.width(), height = image.height(), channels = image.spectrum();
    shared_array<unsigned char> data(new unsigned char[width * height * channels]);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int k = 0; k < channels; ++k) {
                int index = i*width*channels + j*channels + k;
                data[index] = image(j, height - i - 1, 0, k);
            }
        }
    }
    shared_ptr<Texture> texture(new Texture(width, height, channels, data));
    return texture;
}
