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

#ifndef Texture_h
#define Texture_h

#include <string>
#include <GL/gl.h>
#include <boost/shared_array.hpp>

class Texture {
    // Constructors
    public:

    Texture(int width, int height, int channels, boost::shared_array<unsigned char> data);
    ~Texture();

    // Methods
    public:

    GLuint getId() const;
    void bind() const;

    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    boost::shared_array<unsigned char> getData() const;

    static boost::shared_ptr<Texture> fromFile(const std::string &filename);

    // Fields
    private:
    int width, height;
    int channels;
    boost::shared_array<unsigned char> data;
    GLuint id;
};

#endif
