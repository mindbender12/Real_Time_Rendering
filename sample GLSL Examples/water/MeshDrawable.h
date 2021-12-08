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

#ifndef MeshDrawable_h
#define MeshDrawable_h

#include <boost/shared_ptr.hpp>
#include "Drawable.h"

class Mesh;
class Texture;
class CubeMap;
class Shader;

class MeshDrawable : public Drawable {
    // Constructors
    public:
    MeshDrawable(boost::shared_ptr<Mesh> mesh);

    // Methods
    public:
    void setTexture(boost::shared_ptr<Texture> texture);
    void setCubeMap(boost::shared_ptr<CubeMap> cubeMap);
    void setShader(boost::shared_ptr<Shader> shader);
    void setBlend(bool blend);

    protected:
    virtual void drawSelf();

    // Fields
    private:
    boost::shared_ptr<Mesh> mesh;
    boost::shared_ptr<Texture> texture;
    boost::shared_ptr<CubeMap> cubeMap;
    boost::shared_ptr<Shader> shader;
    bool blend;
};

#endif
