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

#ifndef CubeMap_h
#define CubeMap_h

#include <string>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <GL/gl.h>

class CubeMap {
    // Constructors
    public:

    CubeMap(int width, int height, int channels, boost::shared_array<unsigned char> data[6]);
    ~CubeMap();

    // Methods
    public:

    void bind();

    static boost::shared_ptr<CubeMap> fromFiles(const std::string filenames[6]);

    // Fields
    private:

    int width, height;
    int channels;
    GLuint id;
};

#endif
