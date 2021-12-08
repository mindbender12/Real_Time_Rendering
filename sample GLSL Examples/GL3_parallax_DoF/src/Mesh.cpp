#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include "Mesh.h"
#include "VertexBuffer.h"

using std::ios;
using std::ifstream;
using std::string;
using std::istringstream;
using std::invalid_argument;
using std::domain_error;
using std::cout;
using std::endl;


#include "Vector.h" // XXX: for debug purposes only
#include "Timer.h" // XXX: for debug purposes only
Timer t;


Mesh::Mesh(const string& filename) throw(invalid_argument) : _nvert(0), _ntri(0),
	_vertices(NULL), _normals(NULL), _texcoords(NULL), _tangents(NULL), _indices(NULL)
{
	string::size_type loc = filename.find(".ply", 0);

	if (loc != filename.length()) {
		cout << "Loading " << filename << endl;
		t.Reset();
		LoadPLY(filename.c_str());
		t.Stop();
		cout << "PLY loading: " << t.GetInterval() << "ms" << endl;
	}
	else {
		throw invalid_argument("Mesh::Mesh - unrecognized file extension");
	}

	SetupVBO();
}


void Mesh::LoadPLY(const char *filename) throw(domain_error)
{
	int i;

	char buffer[256];
	string str;
	istringstream ss;
	string::size_type pos;

	ifstream is;
	is.open(filename, ios::in);

	int nprop = 0;

	// Parsing the ply identification line
	std::getline(is, str);
	if (str != "ply") {
		throw domain_error("Mesh::LoadPly - Not a PLY file");
	}

	// Parsing the format specification line
	std::getline(is, str);
	if (str != "format ascii 1.0") {
		throw domain_error("Mesh::LoadPly - Not an ASCII PLY file");
	}


	// Parsing number of vertices
	while (is.good()) {
		std::getline(is, str);

		if ( (pos = str.find("element vertex ")) == string::npos ) {
			continue;
		}
		else {
			ss.str( str.substr(string("element vertex ").length()) );
			ss >> _nvert;
			ss.clear(istringstream::goodbit);

			cout << "Vertices: " << _nvert << endl;
			break;
		}
	}


	// Parsing number of vertex properties
	while (is.good()) {
		std::getline(is, str);

		if ( (pos = str.find("property ")) != string::npos ) {
			nprop++;
		}
		else {
			is.seekg(-is.gcount()); // Unget last line
			cout << "Properties: " << nprop << endl;
			break;
		}
	}


	// Parsing number of triangles
	while (is.good()) {
		std::getline(is, str);

		if ( (pos = str.find("element face ")) == string::npos ) {
			continue;
		}
		else {
			ss.str( str.substr(string("element face ").length()) );
			ss >> _ntri;
			ss.clear(istringstream::goodbit);

			cout << "Triangles: " << _ntri << endl;
			break;
		}

	}

	// Parsing end of the header
	while (is.good()) {
		std::getline(is, str);

		if ( (pos = str.find("end_header")) == string::npos ) {
			continue;
		}
		else {
			break;
		}
	}


	_vertices = new GLfloat[_nvert * 3];
	if (nprop >= 6) {
		_normals = new GLfloat[_nvert * 3];
	}
	if (nprop >= 8) {
		_texcoords = new GLfloat[_nvert * 2];
	}

	// Parsing vertices
	for (i = 0; i < _nvert; i++) {
		std::getline(is, str);

		ss.str(str);
		ss >> _vertices[i*3] >> _vertices[i*3 + 1] >> _vertices[i*3 + 2];
		if (nprop >= 6) {
			ss >> _normals[i*3] >> _normals[i*3 + 1] >> _normals[i*3 + 2];
		}
		if (nprop >= 8) {
			ss >> _texcoords[i*2] >> _texcoords[i*2 + 1];
		}
		ss.clear(istringstream::goodbit);

//		cout << "Vertices: " << _vertices[i*3] << " " << _vertices[i*3 + 1] << " " << _vertices[i*3 + 2] << endl;
//		cout << "Normals: " << _normals[i*3] << " " << _normals[i*3 + 1] << " " << _normals[i*3 + 2] << endl;
//		cout << "TexCoords: " << _texcoords[i*2] << " " << _texcoords[i*2 + 1] << endl;
	}

	_indices = new unsigned int[_ntri * 3];

	// Parsing triangles
	int face_vert;
	for (i = 0; i < _ntri; i++) {
		std::getline(is, str);

		ss.str(str);
		ss >> face_vert;
		if (face_vert != 3) {
			throw domain_error("Mesh::LoadPly - Not a triangular face");
		}
		ss >> _indices[i*3] >> _indices[i*3 + 1] >> _indices[i*3 + 2];
		ss.clear(istringstream::goodbit);

//		cout << "Indices: " << _indices[i*3] << " " << _indices[i*3 + 1] << " " << _indices[i*3 + 2] << endl;
	}

	is.close();
}


void Mesh::SetupVBO()
{
	if (_texcoords) {
		_tangents = new GLfloat[_nvert * 3];

//		t.Reset();
//		CalcTangents();
//		t.Stop();
//		cout << "Tangents calculations (w/o classes): " << t.GetInterval() << "ms" << endl;
		t.Reset();
		ClassTangents();
		t.Stop();
		cout << "Tangents calculations (with classes): " << t.GetInterval() << "ms" << endl;


		_vbo.SetTangents(_nvert, _tangents);
		delete[] _tangents;
		_tangents = NULL;
	}


	if (_vertices) {
		_vbo.SetVertices(_nvert, _vertices);
		delete[] _vertices;
		_vertices = NULL;
	}
	
	if (_normals) {
		_vbo.SetNormals(_nvert, _normals);
		delete[] _normals;
		_normals = NULL;
	}

	if (_texcoords) {
		_vbo.SetTexCoords(_nvert, _texcoords);
		delete[] _texcoords;
		_texcoords = NULL;
	}

	if (_indices) {
		 _vbo.SetIndices(_ntri*3, _indices);
		 delete[] _indices;
		 _indices = NULL;
	}

}


void Mesh::ClassTangents()
{
	int i, j;
	int idx[3];
	Vector v[3], u[3];
	Vector e[2], d[2];
	Vector tan;

	for (i = 0; i < _ntri; i++) {

		// Getting indices
		idx[0] = _indices[i*3];
		idx[1] = _indices[i*3 + 1];
		idx[2] = _indices[i*3 + 2];

		// Getting Vertices and texcoords
		for (j = 0; j < 3; j++) {
			v[j].Load(&_vertices[3*idx[j]]);
			u[j].Load(_texcoords[2*idx[j]], _texcoords[2*idx[j] + 1], 1.0);
		}

		// Vertex edges
		e[0] = v[1] - v[0];
		e[1] = v[2] - v[0];
		// Texcoords edges
		d[0] = u[1] - u[0];
		d[1] = u[2] - u[0];

		float cp = d[0].Y() * d[1].X() - d[0].X() * d[1].Y();

		// Calculating tangent
		if (cp != 0.0) {
			float mul = 1.0 / cp;
			tan = (e[0] * (-d[1].Y()) + e[1] * d[0].Y()) * mul;
		}

		// Normalizing tangent
		tan.Normalize();

		// Replicating the tangent for every vertex of a face
		for (j = 0; j < 3; j++) {
			_tangents[3*idx[j]] = tan.X();
			_tangents[3*idx[j] + 1] = tan.Y();
			_tangents[3*idx[j] + 2] = tan.Z();
		}
//		cout << "ClassTangents(): " << tan.X() << " " << tan.Y() << " " << tan.Z() << endl;

	}
}


void Mesh::CalcTangents()
{
	float tan[3];

	int i, j;
	int idx[3];
	float v0[3], v1[3], v2[3];
	float u0[2], u1[2], u2[2];

	for (i = 0; i < _ntri; i++) {

		// Getting indices
		idx[0] = _indices[i*3];
		idx[1] = _indices[i*3 + 1];
		idx[2] = _indices[i*3 + 2];

		// Getting vertices
		for (j = 0; j < 3; j++) {
			v0[j] = _vertices[3*idx[0] + j];
			v1[j] = _vertices[3*idx[1] + j];
			v2[j] = _vertices[3*idx[2] + j];
		}

		// Getting texcoords
		for (j = 0; j < 2; j++) {
			u0[j] = _texcoords[2*idx[0] + j];
			u1[j] = _texcoords[2*idx[1] + j];
			u2[j] = _texcoords[2*idx[2] + j];
		}

		float e0[3], e1[3];
		float d0[2], d1[2];

		// Vertex edges
		for (j = 0; j < 3; j++) {
			e0[j] = v1[j] - v0[j];
			e1[j] = v2[j] - v0[j];
		}

		// Texcoords edges
		for (j = 0; j < 2; j++) {
			d0[j] = u1[j] - u0[j];
			d1[j] = u2[j] - u0[j];
		}

		float cp = d0[1] * d1[0] - d0[0] * d1[1];

		// Calculating tangent
		if (cp != 0.0) {
			float mul = 1.0 / cp;
			for (j = 0; j < 3; j++) {
				tan[j] = (e0[j] * -d1[1] + e1[j] * d0[1]) * mul;
			}
		}

		// Normalizing tangent
		float l = sqrt(tan[0]*tan[0] + tan[1]*tan[1] + tan[2]*tan[2]);
		float mul = 1.0 / l;
		for (j = 0; j < 3; j++) {
			tan[j] *= mul;
		}

		// Replicating the tangent for every vertex of a face
		for (j = 0; j < 3; j++) {
			_tangents[3*idx[0] + j] = tan[j];
			_tangents[3*idx[1] + j] = tan[j];
			_tangents[3*idx[2] + j] = tan[j];
		}
//		cout << "CalcTangents(): " << tan[0] << " " << tan[1] << " " << tan[2] << endl;
	}

//	cout << "Per vertex" << endl;
//	for (i = 0; i < _nvert; i++) {
//		cout << _tangents[i*3] << " " << _tangents[i*3 + 1] << " " << _tangents[i*3 + 2] << endl;
//	}
}


void Mesh::Draw()
{
	_vbo.DrawElements();
}
