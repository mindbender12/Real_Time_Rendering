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

#include "MeshDrawable.h"

#include <vector>
#include <GL/gl.h>
#include <boost/shared_array.hpp>
#include "Controller.h"
#include "Mesh.h"
#include "Texture.h"
#include "CubeMap.h"
#include "Shader.h"
#include "Vector.h"
#include "Utilities.h"
#include "GLView.h"
#include "Camera.h"

using namespace std;
using namespace boost;

MeshDrawable::MeshDrawable(shared_ptr<Mesh> mesh) :
    mesh(mesh),
    blend(false)
{
}

void MeshDrawable::setTexture(shared_ptr<Texture> texture) {
    this->texture = texture;
}

void MeshDrawable::setCubeMap(shared_ptr<CubeMap> cubeMap) {
    this->cubeMap = cubeMap;
}

void MeshDrawable::setShader(shared_ptr<Shader> shader) {
    this->shader = shader;
}

void MeshDrawable::setBlend(bool blend) {
    this->blend = blend;
}

void MeshDrawable::drawSelf() {
    glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT);
    if (cubeMap) {
        glEnable(GL_TEXTURE_CUBE_MAP);
        cubeMap->bind();
    } else if (texture)
        texture->bind();
    else
        glBindTexture(GL_TEXTURE_2D, 0);

    if (shader) {
        if (shader->hasUniform("time"))
            shader->setUniform("time", (float) controller.getElapsedTime());
        if (shader->hasUniform("eyePos")) {
            Vector eyePos = glview.getCamera()->getPosition();
            shader->setUniform("eyePos", eyePos.x(), eyePos.y(), eyePos.z());
        }
        shader->bind();
    }

    if (blend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    if (!mesh->hasVbos()) {
        glBegin(GL_TRIANGLES);
        shared_array<Vector> vertices = mesh->getVertices();
        shared_array<Vector> normals = mesh->getNormals();
        shared_array<Pair> texCoords = mesh->getTexCoords();
        shared_array<int> indices = mesh->getIndices();
        for (int i = 0; i < mesh->getNumTriangles(); ++i) {
            for (int j = 0; j < 3; ++j) {
                int index = indices[3*i + j];
                if (mesh->hasNormals()) {
                    Vector n(normals[index]);
                    glNormal3fv(n.array());
                }
                if (mesh->hasTexCoords()) {
                    Pair p(texCoords[index]);
                    glTexCoord2f(p.u, p.v);
                }
                Vector v(vertices[index]);
                glVertex3fv(v.array());
            }
        }
        glEnd();
    } else {
        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexVbo());
        glVertexPointer(3, GL_FLOAT, 0, 0);
        if (mesh->hasNormals()) {
            glEnableClientState(GL_NORMAL_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalVbo());
            glNormalPointer(GL_FLOAT, 0, 0);
        }
        if (mesh->hasTexCoords()) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, mesh->getTexCoordVbo());
            glTexCoordPointer(2, GL_FLOAT, 0, 0);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexVbo());
        glDrawElements(GL_TRIANGLES, mesh->getNumTriangles() * 3, GL_UNSIGNED_INT, 0);
        glPopClientAttrib();
    }

    glPopAttrib();
    glUseProgram(0);
}
