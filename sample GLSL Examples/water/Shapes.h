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

#ifndef Shapes_h
#define Shapes_h

#include <boost/shared_ptr.hpp>

class Mesh;

boost::shared_ptr<Mesh> makeSquare(float side);
boost::shared_ptr<Mesh> makeCylinder(float radius, float height, int slices);

#endif
