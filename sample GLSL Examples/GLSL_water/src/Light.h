#ifndef CLASS_LIGHT
#define CLASS_LIGHT

#include <stdexcept>
#include <GL/gl.h>
#include <GL/glext.h>

/// Low-level light handling
class Light
{
	private:
		GLfloat _ambient[4];
		GLfloat _diffuse[4];
		GLfloat _specular[4];
		GLfloat _position[4];
		GLfloat _spotdir[3];
		GLfloat _spotexp;
		GLfloat _spotcutoff;
		GLfloat _catt;
		GLfloat _latt;
		GLfloat _qatt;

		GLenum _unit;
		int _max_lights;
	public:
		Light(int unit = 0) throw(std::out_of_range);

		int Unit();
		void Unit(int unit) throw (std::out_of_range);

		GLfloat *Ambient();
		void Ambient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
		void Ambient(GLfloat *ambient);
		GLfloat *Diffuse();
		void Diffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
		void Diffuse(GLfloat *diffuse);
		GLfloat *Specular();
		void Specular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
		void Specular(GLfloat *specular);

		GLfloat *Position();
		void Position(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1.0f);
		void Position(GLfloat *position);

		GLfloat *SpotDir();
		void SpotDir(GLfloat x, GLfloat y, GLfloat z);
		void SpotDir(GLfloat *spotdir);
		GLfloat SpotExp();
		void SpotExp(GLfloat spotexp);
		GLfloat SpotCutOff();
		void SpotCutOff(GLfloat spotcutoff);

		GLfloat CAttenuation();
		void CAttenuation(GLfloat catt);
		GLfloat LAttenuation();
		void LAttenuation(GLfloat latt);
		GLfloat QAttenuation();
		void QAttenuation(GLfloat qatt);

		void Enable();
		void Disable();
};

#endif
