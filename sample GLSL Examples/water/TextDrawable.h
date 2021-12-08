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

#ifndef TextDrawable_h
#define TextDrawable_h

#include <string>
#include <boost/shared_ptr.hpp>
#include "Drawable.h"
#include "Utilities.h"

class Font;

class TextDrawable : public Drawable {
    // Constructors
    public:
    TextDrawable(const std::string &text, boost::shared_ptr<Font> font, Color color);

    // Methods
    public:

    const std::string& getText() const;
    void setText(const std::string &text);

    protected:
    void drawSelf();

    // Fields
    private:
    std::string text;
    boost::shared_ptr<Font> font;
    Color color;
};

#endif
