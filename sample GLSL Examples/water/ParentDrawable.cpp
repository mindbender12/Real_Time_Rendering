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

#include "ParentDrawable.h"

using namespace std;
using namespace boost;

ParentDrawable::ParentDrawable() {}

ParentDrawable::ParentDrawable(const vector<shared_ptr<Drawable> > &children) :
    children(children)
{
}

const vector<shared_ptr<Drawable> >& ParentDrawable::getChildren() const {
    return children;
}

void ParentDrawable::addChild(shared_ptr<Drawable> child) {
    children.push_back(child);
}

void ParentDrawable::removeChild(int index) {
    children.erase(children.begin() + index);
}

void ParentDrawable::drawChildren() {
    for (vector<shared_ptr<Drawable> >::iterator i = children.begin(); i != children.end(); ++i)
        (*i)->draw();
}

void ParentDrawable::drawSelf() {
    drawChildren();
}
