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

#include "Font.h"

#include <stdexcept>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include "Utilities.h"

using namespace std;
using namespace boost;

FT_Library library;

static void initializeFreeType();
static void shutdownFreeType();

Font::Font(const string &filename, float height) :
    height(height),
    textures(new GLuint[128]),
    listBase(0)
{
    int ret;
    FT_Face face;
    ret = FT_New_Face(library, filename.c_str(), 0, &face);
    if (ret)
        throw new runtime_error("FT_New_Face failed");

    FT_Set_Char_Size(face, height * 64, height * 64, 96, 96);

    listBase = glGenLists(128);
    glGenTextures(128, textures.get());
    glPushAttrib(GL_TEXTURE_BIT);
    for (int i = 0; i < 128; ++i)
        makeCharacter(face, (char) i, listBase + i, textures[i]);
    glPopAttrib();

    FT_Done_Face(face);
}

Font::~Font() {
    glDeleteTextures(128, textures.get());
    glDeleteLists(listBase, 128);
}

void Font::drawCharacter(char ch) {
    setGL();
    glCallList(ch);
    unsetGL();
}

void Font::drawString(const string &str) {
    setGL();
    glCallLists(str.length(), GL_UNSIGNED_BYTE, str.c_str());
    unsetGL();
}

void Font::makeCharacter(FT_Face face, char ch, GLuint list, GLuint texture) {
    int ret;
    ret = FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT);
    if (ret)
        throw runtime_error("FT_Load_Glyph failed");

    FT_Glyph glyph;
    ret = FT_Get_Glyph(face->glyph, &glyph);
    if (ret)
        throw runtime_error("FT_Get_Glyph failed");

    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
    FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph) glyph;
    FT_Bitmap &bitmap = bitmapGlyph->bitmap;

    int width = padToPowerOfTwo(bitmap.width);
    int height = padToPowerOfTwo(bitmap.rows);
    shared_array<GLubyte> texels(new GLubyte[width * height * 2]);
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            int si = r * bitmap.width + c;
            int di = r * width * 2 + c * 2;
            GLubyte color = (c >= bitmap.width || r >= bitmap.rows) ?
                0 : bitmap.buffer[si];
            texels[di] = texels[di + 1] = color;
        }
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0,
                 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, texels.get());

    glNewList(list, GL_COMPILE);

    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
    glTranslatef(bitmapGlyph->left, 0.0, 0.0);
    glTranslatef(0.0, bitmapGlyph->top - bitmap.rows, 0.0);

    float x = (float) bitmap.width / (float) width;
    float y = (float) bitmap.rows / (float) height;
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(0.0, bitmap.rows);
    glTexCoord2f(0.0, y);
    glVertex2f(0.0, 0.0);
    glTexCoord2f(x, y);
    glVertex2f(bitmap.width, 0.0);
    glTexCoord2f(x, 0.0);
    glVertex2f(bitmap.width, bitmap.rows);
    glEnd();

    glPopMatrix();
    glTranslatef(face->glyph->advance.x / 64, 0.0, 0.0);

    glEndList();
}   

void Font::setGL() {
    float lineHeight = height / .63;
    glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glListBase(listBase);
    glPushMatrix();
}

void Font::unsetGL() {
    glPopAttrib();
    glPopMatrix();
}

__attribute__((constructor)) static void initializeFreeType() {
    int ret = FT_Init_FreeType(&library);
    if (ret)
        throw runtime_error("FT_Init_FreeType failed");
}

__attribute__((destructor)) static void shutdownFreeType() {
    FT_Done_FreeType(library);
}


