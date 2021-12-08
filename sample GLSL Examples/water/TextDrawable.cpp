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

#include "TextDrawable.h"

#include "Font.h"

using namespace std;
using namespace boost;

TextDrawable::TextDrawable(const string &text, shared_ptr<Font> font, Color color) :
    text(text),
    font(font),
    color(color)
{
}

const string& TextDrawable::getText() const {
    return text;
}

void TextDrawable::setText(const string &text) {
    this->text = text;
}

void TextDrawable::drawSelf() {
    glPushAttrib(GL_CURRENT_BIT);
    glColor4fv(color.array());
    font->drawString(text);
    glPopAttrib();
}
