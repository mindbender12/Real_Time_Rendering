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

#ifndef ParentDrawable_h
#define ParentDrawable_h

#include <vector>
#include <boost/shared_ptr.hpp>
#include "Drawable.h"

class ParentDrawable : public Drawable {
    // Constructors
    public:
    ParentDrawable();
    ParentDrawable(const std::vector<boost::shared_ptr<Drawable> > &children);

    // Methods
    const std::vector<boost::shared_ptr<Drawable> >& getChildren() const;
    void addChild(boost::shared_ptr<Drawable> child);
    void removeChild(int index);

    protected:
    virtual void drawChildren();
    virtual void drawSelf();

    // Fields
    protected:
    std::vector<boost::shared_ptr<Drawable> > children;
};
    

#endif
