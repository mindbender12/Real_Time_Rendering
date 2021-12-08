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

#ifndef Skybox_h
#define Skybox_h

#include <boost/shared_ptr.hpp>
#include "Drawable.h"

class Texture;

class Skybox : public Drawable {
    // Constructor
    public:

    Skybox(boost::shared_ptr<Texture> north,
           boost::shared_ptr<Texture> east,
           boost::shared_ptr<Texture> south,
           boost::shared_ptr<Texture> west,
           boost::shared_ptr<Texture> up,
           boost::shared_ptr<Texture> down);

    // Methods
    public:

    virtual void drawSelf();

    // Fields
    private:
    boost::shared_ptr<Texture> north, east, south, west, up, down;
};

#endif
