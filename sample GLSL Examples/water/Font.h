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

#ifndef Font_h
#define Font_h

#include <string>
#include <boost/shared_array.hpp>
#include <GL/gl.h>
#include <ft2build.h>
#include <freetype/freetype.h>

class Font {
    // Constructors
    public:
    Font(const std::string &filename, float height);
    ~Font();

    // Methods
    public:
    void drawCharacter(char ch);
    void drawString(const std::string &str);

    private:
    void makeCharacter(FT_Face face, char ch, GLuint list, GLuint texture);
    void setGL();
    void unsetGL();

    // Fields
    private:
    float height;
    boost::shared_array<GLuint> textures;
    GLuint listBase;
};

#endif
