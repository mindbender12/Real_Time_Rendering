#include <stdexcept>
#include "Light.h"

using std::out_of_range;

Light::Light(int unit) throw (out_of_range)
{
	glGetIntegerv(GL_MAX_LIGHTS, &_max_lights);
	_max_lights = 8; // FIXME: glGetIntergv returns the wrong value

	if (unit >= _max_lights) {
		throw out_of_range("Light::Light - GL_MAX_LIGHTS exceeded");
	}
	_unit = GL_LIGHT0 + unit;

	Ambient(0.0f, 0.0f, 0.0f, 1.0f);
	Diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	Specular(1.0f, 1.0f, 1.0f, 1.0f);

	Position(0.0f, 0.0f, 1.0f, 0.0f);

	SpotDir(0.0f, 0.0f, -1.0f);
	SpotExp(0.0f);
	SpotCutOff(180.0f);

	CAttenuation(1.0f);
	LAttenuation(0.0f);
	QAttenuation(0.0f);
}


int Light::Unit()
{
	return _unit - GL_LIGHT0;
}


void Light::Unit(int unit) throw (out_of_range)
{
	if (unit >= _max_lights) {
		throw out_of_range("Light::Light - GL_MAX_LIGHTS exceeded");
	}
	_unit = GL_LIGHT0 + unit;

	glLightfv(_unit, GL_AMBIENT, _ambient);
	glLightfv(_unit, GL_DIFFUSE, _diffuse);
	glLightfv(_unit, GL_SPECULAR, _specular);
	glLightfv(_unit, GL_POSITION, _position);
	glLightfv(_unit, GL_SPOT_DIRECTION, _spotdir);
	glLightf(_unit, GL_SPOT_EXPONENT, _spotexp);
	glLightf(_unit, GL_SPOT_CUTOFF, _spotcutoff);
	glLightf(_unit, GL_CONSTANT_ATTENUATION, _catt);
	glLightf(_unit, GL_LINEAR_ATTENUATION, _latt);
	glLightf(_unit, GL_QUADRATIC_ATTENUATION, _qatt);
}


GLfloat *Light::Ambient()
{
	return _ambient;
}


void Light::Ambient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	_ambient[0] = r;
	_ambient[1] = g;
	_ambient[2] = b;
	_ambient[3] = a;

	glLightfv(_unit, GL_AMBIENT, _ambient);
}


void Light::Ambient(GLfloat *ambient)
{
	_ambient[0] = ambient[0];
	_ambient[1] = ambient[1];
	_ambient[2] = ambient[2];
	_ambient[3] = ambient[3];

	glLightfv(_unit, GL_AMBIENT, _ambient);
}


GLfloat *Light::Diffuse()
{
	return _diffuse;
}


void Light::Diffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	_diffuse[0] = r;
	_diffuse[1] = g;
	_diffuse[2] = b;
	_diffuse[3] = a;

	glLightfv(_unit, GL_DIFFUSE, _diffuse);
}


void Light::Diffuse(GLfloat *diffuse)
{
	_diffuse[0] = diffuse[0];
	_diffuse[1] = diffuse[1];
	_diffuse[2] = diffuse[2];
	_diffuse[3] = diffuse[3];

	glLightfv(_unit, GL_DIFFUSE, _diffuse);
}


GLfloat *Light::Specular()
{
	return _specular;
}


void Light::Specular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	_specular[0] = r;
	_specular[1] = g;
	_specular[2] = b;
	_specular[3] = a;

	glLightfv(_unit, GL_SPECULAR, _specular);
}


void Light::Specular(GLfloat *specular)
{
	_specular[0] = specular[0];
	_specular[1] = specular[1];
	_specular[2] = specular[2];
	_specular[3] = specular[3];

	glLightfv(_unit, GL_SPECULAR, _specular);
}


GLfloat *Light::Position()
{
	return _position;
}


void Light::Position(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	_position[0] = x;
	_position[1] = y;
	_position[2] = z;
	_position[3] = w;

	glLightfv(_unit, GL_POSITION, _position);
}


void Light::Position(GLfloat *position)
{
	_position[0] = position[0];
	_position[1] = position[1];
	_position[2] = position[2];
	_position[3] = position[3];

	glLightfv(_unit, GL_POSITION, _position);
}


GLfloat *Light::SpotDir()
{
	return _spotdir;
}


void Light::SpotDir(GLfloat x, GLfloat y, GLfloat z)
{
	_spotdir[0] = x;
	_spotdir[1] = y;
	_spotdir[2] = z;

	glLightfv(_unit, GL_SPOT_DIRECTION, _spotdir);
}


void Light::SpotDir(GLfloat *spotdir)
{
	_spotdir[0] = spotdir[0];
	_spotdir[1] = spotdir[1];
	_spotdir[2] = spotdir[2];

	glLightfv(_unit, GL_SPOT_DIRECTION, _spotdir);
}


GLfloat Light::SpotExp()
{
	return _spotexp;
}


void Light::SpotExp(GLfloat spotexp)
{
	_spotexp = spotexp;

	glLightf(_unit, GL_SPOT_EXPONENT, _spotexp);
}


GLfloat Light::SpotCutOff()
{
	return _spotcutoff;
}


void Light::SpotCutOff(GLfloat spotcutoff)
{
	_spotcutoff = spotcutoff;

	glLightf(_unit, GL_SPOT_CUTOFF, _spotcutoff);
}


GLfloat Light::CAttenuation()
{
	return _catt;
}


void Light::CAttenuation(GLfloat catt)
{
	_catt = catt;

	glLightf(_unit, GL_CONSTANT_ATTENUATION, _catt);
}


GLfloat Light::LAttenuation()
{
	return _latt;
}


void Light::LAttenuation(GLfloat latt)
{
	_latt = latt;

	glLightf(_unit, GL_LINEAR_ATTENUATION, _latt);
}


GLfloat Light::QAttenuation()
{
	return _qatt;
}


void Light::QAttenuation(GLfloat qatt)
{
	_qatt = qatt;

	glLightf(_unit, GL_QUADRATIC_ATTENUATION, _qatt);
}


void Light::Enable()
{
	glEnable(_unit);
}


void Light::Disable()
{
	glDisable(_unit);
}
