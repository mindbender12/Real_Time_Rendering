#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include "MeshLoader.h"
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


VertexBuffer *MeshLoader::Load(const string& filename, VertexBuffer *vbo) throw(invalid_argument)
{	
	string::size_type loc = filename.find(".ply", 0);

	if (loc != filename.length()) {
		cout << "Loading " << filename << endl;
		t.Start();
		LoadPLY(filename.c_str());
		t.Stop();
		cout << "PLY loading: " << t.GetInterval() << "ms" << endl;
	}
	else {
		throw invalid_argument("MeshLoader::MeshLoader - unrecognized file extension");
	}

	if (vbo)
		_vbo = vbo;
	else
		_vbo = new VertexBuffer();

	SetupVBO();
	
	return _vbo;
}


void MeshLoader::LoadPLY(const char *filename) throw(domain_error)
{
	int i;

	string str;
	istringstream ss;
	string::size_type pos;

	ifstream is;
	is.open(filename, ios::in);

	int nprop = 0;

	// Parsing the ply identification line
	std::getline(is, str);
	if (str != "ply") {
		throw domain_error("MeshLoader::LoadPly - Not a PLY file");
	}

	// Parsing the format specification line
	std::getline(is, str);
	if (str != "format ascii 1.0") {
		throw domain_error("MeshLoader::LoadPly - Not an ASCII PLY file");
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
			throw domain_error("MeshLoader::LoadPly - Not a triangular face");
		}
		ss >> _indices[i*3] >> _indices[i*3 + 1] >> _indices[i*3 + 2];
		ss.clear(istringstream::goodbit);

//		cout << "Indices: " << _indices[i*3] << " " << _indices[i*3 + 1] << " " << _indices[i*3 + 2] << endl;
	}

	is.close();
}


void MeshLoader::SetupVBO()
{
	if (_texcoords) {
		_tangents = new GLfloat[_nvert * 4];

		t.Start();
		CalcTangents();
		t.Stop();
		cout << "Tangents calculation: " << t.GetInterval() << "ms" << endl;


		_vbo->SetTangents(_nvert, _tangents);
		delete[] _tangents;
		_tangents = NULL;
	}


	if (_vertices) {
		_vbo->SetVertices(_nvert, _vertices);
		delete[] _vertices;
		_vertices = NULL;
	}
	
	if (_normals) {
		_vbo->SetNormals(_nvert, _normals);
		delete[] _normals;
		_normals = NULL;
	}

	if (_texcoords) {
		_vbo->SetTexCoords(_nvert, _texcoords);
		delete[] _texcoords;
		_texcoords = NULL;
	}

	if (_indices) {
		 _vbo->SetIndices(_ntri*3, _indices);
		 delete[] _indices;
		 _indices = NULL;
	}
	
	_nvert = _ntri = 0;
}


// Implementation of http://www.terathon.com/code/tangent.html
void MeshLoader::CalcTangents()
{
	int i, j;
	int idx[3];
	Vector v[3], u[3];
	Vector e[2], d[2];
	Vector tan, bitan;

    for (i = 0; i < _ntri; i++) {
		// Getting indices
		idx[0] = _indices[i*3];
		idx[1] = _indices[i*3 + 1];
		idx[2] = _indices[i*3 + 2];

       	// Getting Vertices and texcoords
		for (j = 0; j < 3; j++) {
			v[j].Load(&_vertices[3*idx[j]]);
			u[j].Load(_texcoords[2*idx[j]], _texcoords[2*idx[j] + 1], 1.0f);
		}

   		// Vertex edges
		e[0] = v[1] - v[0];
		e[1] = v[2] - v[0];
		// Texcoords edges
		d[0] = u[1] - u[0];
		d[1] = u[2] - u[0];
 
		float r = 1.0f/(d[0].X() * d[1].Y() - d[1].X() * d[0].Y());
		tan = (e[0]*d[1].Y() + e[1]*(-d[0].Y())) * r;
		bitan = (e[1]*d[0].X() + e[0]*(-d[1].X())) * r;

		// Replicating the tangent for every vertex of a face
		for (j = 0; j < 3; j++) {
			_tangents[4*idx[j] + 0] = tan.X();
			_tangents[4*idx[j] + 1] = tan.Y();
			_tangents[4*idx[j] + 2] = tan.Z();
		}
	}

	for (i = 0; i < _nvert; i++) {
		Vector normal(&_normals[3*i]);
		Vector tangent(&_tangents[4*i]);

		// Gram-Schmidt orthogonalize
        tangent = (tangent - normal * (normal * tangent)).Normalize();

		_tangents[4*i + 0] = tangent.X();
		_tangents[4*i + 1] = tangent.Y();
		_tangents[4*i + 2] = tangent.Z();
		// Calculate handedness
        _tangents[4*i + 3] = (bitan * (normal % tangent) < 0.0f) ? -1.0f : 1.0f;
	}

//		cout << "ClassTangents(): " << tan.X() << " " << tan.Y() << " " << tan.Z() << endl;
}
