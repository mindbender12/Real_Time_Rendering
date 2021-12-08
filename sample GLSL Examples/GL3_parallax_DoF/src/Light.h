#ifndef CLASS_LIGHT
#define CLASS_LIGHT

#include <GL/gl.h>
#include <GL/glext.h>

/// Low-level light handling
class Light
{
	private:
		GLfloat _position[4];
		GLfloat _ambient[4];
		GLfloat _diffuse[4];
		GLfloat _specular[4];
		GLfloat _catt;
		GLfloat _latt;
		GLfloat _qatt;
	public:
		Light();

		GLfloat *Position();
		void Position(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0);
		GLfloat *Ambient();
		void Ambient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0);
		GLfloat *Diffuse();
		void Diffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0);
		GLfloat *Specular();
		void Specular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0);

		GLfloat CAttenuation();
		void CAttenuation(GLfloat catt);
		GLfloat LAttenuation();
		void LAttenuation(GLfloat latt);
		GLfloat QAttenuation();
		void QAttenuation(GLfloat qatt);
};

#endif

