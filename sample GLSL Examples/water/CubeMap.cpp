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

#include "CubeMap.h"

#include <CImg.h>

using namespace std;
using namespace boost;
using namespace cimg_library;

CubeMap::CubeMap(int width, int height, int channels, shared_array<unsigned char> data[6]) :
    width(width),
    height(height),
    channels(channels)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
    for (int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0,
                     channels == 3 ? GL_RGB8 : GL_RGBA8,
                     width,
                     height,
                     0,
                     channels == 3 ? GL_RGB : GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     data[i].get());
    }
}

CubeMap::~CubeMap() {
    glDeleteTextures(1, &id);
}

void CubeMap::bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

shared_ptr<CubeMap> CubeMap::fromFiles(const string filenames[6]) {
    int width, height, channels;
    shared_array<unsigned char> data[6];
    for (int i = 0; i < 6; ++i) {
        CImg<unsigned char> image(filenames[i].c_str());
        if (i == 0) {
            width = image.width();
            height = image.height();
            channels = image.spectrum();
        } else {
            assert(image.width() == width);
            assert(image.height() == height);
            assert(image.spectrum() == channels);
        }

        data[i].reset(new unsigned char[width * height * channels]);
        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                for (int k = 0; k < channels; ++k) {
                    int index = r*width*channels + c*channels + k;
                    data[i][index] = image(c, height - r - 1, k);
                }
            }
        }
    }
    shared_ptr<CubeMap> cubeMap(new CubeMap(width, height, channels, data));
    return cubeMap;
}
