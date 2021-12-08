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

#ifndef Shader_h
#define Shader_h

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include <GL/gl.h>

struct Uniform;

class Shader {
    // Constructors
    public:
    Shader(const std::string &vertexSource, const std::string &fragmentSource);
    ~Shader();

    // Methods
    public:

    void bind();
    bool hasUniform(const std::string &name);
    void setUniform(const std::string &name, int v1);
    void setUniform(const std::string &name, float v1);
    void setUniform(const std::string &name, float v1, float v2);
    void setUniform(const std::string &name, float v1, float v2, float v3);
    void setUniform(const std::string &name, float v1, float v2, float v3, float v4);

    static boost::shared_ptr<Shader> fromFiles(const std::string &vertexFilename,
                                               const std::string &fragmentFilename);

    private:
    void compileAndAttach(GLenum which, GLuint *shader, const std::string &source);

    // Fields
    private:
    GLuint program;
    GLuint vertexShader, fragmentShader;
    std::map<std::string, Uniform> uniforms;
};

#endif
