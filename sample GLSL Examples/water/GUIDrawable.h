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

#ifndef GUIDrawable_h
#define GUIDrawable_h

#include <vector>
#include <boost/shared_ptr.hpp>
#include "Drawable.h"
#include "ParentDrawable.h"

class GUIDrawable : public ParentDrawable {
    // Constructors
    public:
    GUIDrawable();
    GUIDrawable(const std::vector<boost::shared_ptr<Drawable> > &children);

    // Methods
    protected:
    virtual void drawSelf();
};

#endif
