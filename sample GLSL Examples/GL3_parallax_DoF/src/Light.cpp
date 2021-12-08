#include "Light.h"

Light::Light()
{
	_position[0] = _position[1] = _position[2] = 0.0;
	_position[3] = 1.0;

	_ambient[0] = _ambient[1] = _ambient[2] = _ambient[3] = 0.2;
	_diffuse[0] = _diffuse[1] =  _diffuse[2] =  _diffuse[3] = 1.0;
	_specular[0] = _specular[1] = _specular[2] = _specular[3] = 1.0;

	_catt = 1.0;
	_latt = 0.0;
	_qatt = 0.0;
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
}


GLfloat Light::CAttenuation()
{
	return _catt;
}


void Light::CAttenuation(GLfloat catt)
{
	_catt = catt;
}


GLfloat Light::LAttenuation()
{
	return _latt;
}


void Light::LAttenuation(GLfloat latt)
{
	_latt = latt;
}


GLfloat Light::QAttenuation()
{
	return _qatt;
}


void Light::QAttenuation(GLfloat qatt)
{
	_qatt = qatt;
}
