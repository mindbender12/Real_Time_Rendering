///////////////////////////////////////////////////////////////////////
//          CSE 5542 Course                                          //
//          Lab3                                                     //
//          Implemented by: Soumya Dutta                             //
//          Graduate Student                                         //
//          duttas@cse.ohio-state.edu                                //
//          GLSL Shader Programming                                  //
///////////////////////////////////////////////////////////////////////

// Compile: g++ main_file.cpp -o out -w -I/home/sdutta/glm -lGL -lGLU -lGLEW -lglut
// Run: ./out ../ply_files/Motorbike.ply ../ply_files/Vishnu.ply
// Tested on Dec 08, 2021, in Ubuntu 18.04
// **********************************************************************************

#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtx/transform2.hpp>
#include <stack>
#include "ply.h" // commenting this works fine, but why?
#include "ply.cpp" // WHy we need this to include? ply.h should just work!
#include "shaderSetup.cpp"

using namespace glm;
using namespace std;

const float PI=3.14f;

//Ply stuff
typedef struct VertexPly {
	float x, y, z;
	float nx, ny, nz;
} VertexPly;

typedef struct Face {
	unsigned int count;
	unsigned int *vertices;
	float nx, ny, nz;
} Face;

char* string_list[] = 
{
	"x", "y", "z", "nx", "ny", "nz", "vertex_indices"
};

float cx, cy, cz; 
float x_min, x_max, y_min, y_max, z_min, z_max; 
VertexPly** vertices[20];
Face** faces[20];
unsigned int vertexcount[20];
unsigned int facecount[20];
int vertexnormals[20];
int facenormals[20];
bool has_normal[20];
//ply stuff ends

//Func prototypes
void builRobot(GLuint);
void draw_sphere();
void draw_cude();
void draw_cylinder();
void draw_cone();

typedef struct 
{
	float location[4]; 
	float normal[4];
	float color[4];
} Vertex; 

int cylinderindices,cubeindices,coneindices,sphereindices; 
Vertex *cyverts;   // cylinder vertices
Vertex *spverts;   // sphere vertices
Vertex *cnverts;   // cone vertices
Vertex *cbverts;   //cube vertices
Vertex *plyverts[20]; //plyVerts
GLuint *cyindices,*cnindices,*cbindices,*spindices,*plyindices[20];
GLuint vboHandle[20];   //VBO that contains interleaved positions and colors and normals
GLuint indexVBO[20]; 
float rotAngle=0; 
float rotCone=0;
int rotAngle1=0;
int rotateAll=0;
float walkLen=0;
float walkLen1=0;
int walk=1;
float turn=0;
float rotLArm=0;
float rotRArm=0;
int rotL=0;
int rotR=0;
int moveBack=1;
bool movement = true;
int width=800; int height = 800;
int rotating=0;
float pos=0;
bool rotHead=false;
int rotHeadDir=0;
float rotHeadAngle=0;
mat4 model = mat4(1.0f);
stack<mat4> modelStack; //Global Stack for keeping track of the model matrix
bool polygonMode=false; 
GLuint SetupGLSL(char*);
GLuint c0,c1,c2,p0,p1;
float rot=0;
mat4 XX = mat4(1.0f);
mat4 normalMatrix = mat4(1.0);
mat4 view = mat4(1.0);
GLuint l1On=0;
GLuint l2On=0;
GLuint m1=0;
GLuint m2=0;
GLuint m3=0;
GLuint m4=0;
GLuint m5=0; //diffuse color of object
GLuint m6=0; //kamb
GLuint m7=0; //kdiff
GLuint m8=0; //kspec
GLuint m9=0; //shininess
vec4 diffuseTerm;
vec4 specularTerm;
GLuint programObject;
GLuint toonShader;
GLuint perFragPhong;
GLuint perVertexPhong;
GLuint flat;
int shadeType=0;
float Xcam=0; float Ycam=0; float Zcam=0;
float kspec=0.7; float kdiff=0.6; float kamb=0.2;
int shininess =40;
int light1=1;
int light2=1;


//////////////////////////////////
int totObj; // totObj = number of ply files read
vec4 *randomObjColor;
vec3 *plyObjPos;
vec3 *plyObjScale;
float *plyObjRotate;
vec3 *plyObjRotateAxis;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void store_ply(PlyFile* input, VertexPly ***vertices, Face ***faces,unsigned int* vertexcount, unsigned int* facecount,	int* vertexnormals, int* facenormals, bool* has_normal) 
{
	int i, j;

	// go through the element types
	for(i = 0; i < input->num_elem_types; i++) 
	{
		int count;

		// setup the element for reading and get the element count
		char* element = setup_element_read_ply(input, i, &count);

		// get vertices
		if(strcmp("vertex", element) == 0) 
		{
			*vertices = (VertexPly**)malloc(sizeof(VertexPly) * count);
			*vertexcount = count;

			// run through the properties and store them
			for(j = 0; j < input->elems[i]->nprops; j++) 
			{
				PlyProperty* property = input->elems[i]->props[j];
				PlyProperty setup;

				*has_normal=false;

				if(strcmp("x", property->name) == 0 && property->is_list == PLY_SCALAR) 
				{

					setup.name = string_list[0];
					setup.internal_type = Float32;
					setup.offset = offsetof(VertexPly, x);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
				}
				else if(strcmp("y", property->name) == 0 && property->is_list == PLY_SCALAR) 
				{

					setup.name = string_list[1];
					setup.internal_type = Float32;
					setup.offset = offsetof(VertexPly, y);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
				}
				else if(strcmp("z", property->name) == 0 && property->is_list == PLY_SCALAR) 
				{

					setup.name = string_list[2];
					setup.internal_type = Float32;
					setup.offset = offsetof(VertexPly, z);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
				}
				else if(strcmp("nx", property->name) == 0 && property->is_list == PLY_SCALAR) 
				{

					setup.name = string_list[3];
					setup.internal_type = Float32;
					setup.offset = offsetof(VertexPly, nx);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					*vertexnormals = 1;

					*has_normal=true;
				}
				else if(strcmp("ny", property->name) == 0 && property->is_list == PLY_SCALAR) 
				{

					setup.name = string_list[4];
					setup.internal_type = Float32;
					setup.offset = offsetof(VertexPly, ny);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					*vertexnormals = 1;
				}
				else if(strcmp("nz", property->name) == 0 && property->is_list == PLY_SCALAR) 
				{

					setup.name = string_list[5];
					setup.internal_type = Float32;
					setup.offset = offsetof(VertexPly, nz);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					*vertexnormals = 1;
				}

				// dunno what it is
				else {
					fprintf(stderr, "unknown property type found in %s: %s\n",
						element, property->name);
				}
			}

			// do this if you want to grab the other data
			// list_pointer = get_other_properties_ply
			//                (input, offsetof(Vertex, struct_pointer));

			// copy the data
			for(j = 0; j < count; j = j + 1) 
			{
				(*vertices)[j] = (VertexPly*)malloc(sizeof(VertexPly));

				get_element_ply(input, (void*)((*vertices)[j]));
			}
		}


		// get faces
		else if(strcmp("face", element) == 0) {
			*faces = (Face**)malloc(sizeof(Face) * count);
			*facecount = count;

			// run through the properties and store them
			for(j = 0; j < input->elems[i]->nprops; j = j + 1) 
			{
				PlyProperty* property = input->elems[i]->props[j];
				PlyProperty setup;

				if(strcmp("vertex_indices", property->name) == 0 && property->is_list == PLY_LIST) 
				{

					setup.name = string_list[6];
					setup.internal_type = Uint32;
					setup.offset = offsetof(Face, vertices);
					setup.count_internal = Uint32;
					setup.count_offset = offsetof(Face, count);

					setup_property_ply(input, &setup);
				}
				else if(strcmp("nx", property->name) == 0 && property->is_list == PLY_SCALAR) 
				{

					setup.name = string_list[3];
					setup.internal_type = Float32;
					setup.offset = offsetof(Face, nx);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					*facenormals = 1;
				}
				else if(strcmp("ny", property->name) == 0 && property->is_list == PLY_SCALAR)
				{

					setup.name = string_list[4];
					setup.internal_type = Float32;
					setup.offset = offsetof(Face, ny);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					*facenormals = 1;
				}
				else if(strcmp("nz", property->name) == 0 && property->is_list == PLY_SCALAR) 
				{

					setup.name = string_list[5];
					setup.internal_type = Float32;
					setup.offset = offsetof(Face, nz);
					setup.count_internal = 0;
					setup.count_offset = 0;

					setup_property_ply(input, &setup);
					*facenormals = 1;
				}
				// dunno what it is
				else {
					fprintf(stderr, "unknown property type found in %s: %s\n",
						element, property->name);
				}
			}

			// do this if you want to grab the other data
			// list_pointer = get_other_properties_ply
			//                (input, offsetof(Face, struct_pointer));

			// copy the data
			for(j = 0; j < count; j++) 
			{
				(*faces)[j] = (Face*)malloc(sizeof(Face));

				get_element_ply(input, (void*)((*faces)[j]));
			}
		}

		// who knows?
		else {
			fprintf(stderr, "unknown element type found: %s\n", element);
		}
	}

	// if you want to grab the other data do this
	// get_other_element_ply(input);
}


/////////////////////////////////
// glut mouse control 
////////////////////////////////

int xform_mode = 0; 
#define XFORM_NONE    0 
#define XFORM_ROTATE  1
#define XFORM_SCALE 2 

int press_x, press_y; 
int release_x, release_y; 
float x_angle = 0.0; 
float y_angle = 0.0; 
float scale_size = 1; 


void buildRobot()
{
	modelStack.push(model); //main  	  

	XX=model;
	model=rotate(model,rotHeadAngle,vec3(0,1,0));
	model=translate(model,vec3(0,14,0));
	model=scale(model,vec3(1.5,2.5,1));
	diffuseTerm = vec4(0.8,0,0.2,1);
	normalMatrix=transpose(inverse(view*model));
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	//glUniform4fv(m6, 1,&specularTerm[0]);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	draw_sphere(); //head	  

	XX=translate(XX,vec3(0,14,0));
	XX=scale(XX,vec3(1.5,2.5,1));
	model=XX;

	model=translate(model,vec3(0,-2.5,0));
	model=scale(model,vec3(1.8,1.6,2));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0,0.2,1);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_sphere(); //body

	model=translate(model,vec3(0,-0.9,0));
	model=scale(model,vec3(1.2,0.3,1));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.5,0,0.5,1);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);	
	draw_sphere(); //heap

	model=translate(model,vec3(0,6,0));
	model=scale(model,vec3(1.1,0.3,0.8));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.5,0,0.5,1);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);	
	draw_sphere(); //upper body


	//Leg section*******************************
	modelStack.push(model);	 //for left leg    


	if(walk==0 && movement) //ok
	{		
		model=rotate(model,-rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,walkLen1));
	}	

	else if(walk==1 && movement) //ok
	{
		model=rotate(model,rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,-walkLen1));
	}

	if(moveBack==0 && !movement) //ok
	{	
		model=rotate(model,-rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,walkLen1));
	}	

	else if(moveBack==1 && !movement) //ok
	{

		model=rotate(model,rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,-walkLen1));		
	}

	model=translate(model,vec3(-0.5,-28,0));	 
	model=scale(model,vec3(0.2,12,0.7));	  
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0,0,0.5,1);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_sphere(); //left leg
	model=modelStack.top(); //end for left leg

	modelStack.pop();

	modelStack.push(model);	 //for right leg


	if(walk==0 && movement) //ok
	{
		model=rotate(model,rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,-walkLen1));
	}
	else if(walk==1 && movement) //ok
	{
		model=rotate(model,-rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,walkLen1));
	}

	if(moveBack==0 && !movement) //ok
	{
		model=rotate(model,rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,-walkLen1));
	}
	else if(moveBack==1 && !movement) //ok
	{	
		model=rotate(model,-rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,walkLen1));
	}

	model=translate(model,vec3(0.5,-28,0));
	model=scale(model,vec3(0.2,12,0.7));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0,0,0.5,1);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_sphere(); //right leg
	model=modelStack.top(); //end for right leg

	modelStack.pop();

	//Leg ends**************************************


	//hand section*********************************
	modelStack.push(model);	 //for left hand	  

	if(walk==0 && movement) //ok
	{		 
		model=rotate(model,rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,-walkLen));
	}	

	else if(walk==1 && movement) //ok
	{		 
		model=rotate(model,-rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,walkLen));
	}

	if(moveBack==0 && !movement) //ok 
	{		 
		model=rotate(model,rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,-walkLen));
	}	

	else if(moveBack==1 && !movement) //ok
	{
		model=rotate(model,-rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,walkLen));
	}

	if(rotL==1) //ok
	{
		model=rotate(model,-rotLArm,vec3(1,0,0));
		model=translate(model,vec3(0,-0.1,-0.4));		  
	}

	model=translate(model,vec3(-0.8,-9.5,0));
	model=scale(model,vec3(0.2,10,0.7));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0,0,0.5,1);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_sphere(); //left hand
	model=modelStack.top(); //end for left hand

	modelStack.pop();

	modelStack.push(model);	 //for right hand	 

	if(walk==0 && movement) //ok
	{
		model=rotate(model,-rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,walkLen));
	}	

	else if(walk==1 && movement) //ok
	{
		model=rotate(model,rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,-walkLen));
	}

	if(moveBack==0 && !movement) //ok
	{		  
		model=rotate(model,-rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,walkLen));
	}	

	else if(moveBack==1 && !movement) //ok 
	{

		model=rotate(model,rotAngle,vec3(1,0,0));
		model=translate(model,vec3(0,0,-walkLen));
	}

	if(rotR==1) //ok
	{
		model=rotate(model,-rotRArm,vec3(1,0,0));
		model=translate(model,vec3(0,-0.1,-0.4));
	}

	model=translate(model,vec3(0.8,-9.5,0));
	model=scale(model,vec3(0.2,10,0.7));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0,0,0.5,1);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_sphere(); //right hand
	model=modelStack.top(); //end for right hand

	modelStack.pop();

	model=modelStack.top(); //main ends
}

//Robot Structure
struct Robot 
{
	vec3 pos;
	float angle;

	void moveForward(float dist)
	{
		vec3 original_forward = vec3(0,0,1);
		vec3 forward = vec3(sin(PI*angle/180), 0, cos(PI*angle/180));	
		pos += forward * dist;
	}

	void moveBackward(float dist)
	{
		vec3 original_forward = vec3(0,0,1);
		vec3 forward = vec3(sin(PI*angle/180), 0, cos(PI*angle/180));
		pos -= forward * dist;
	}

	void turnRight(float deg)
	{
		angle += deg;
	}

	void turnLeft(float deg)
	{
		angle -= deg;
	}

	void draw(float startx,float starty,float startz) // pass the starting location of the robot
	{
		modelStack.push(model);	
		model=translate(model,vec3(pos[0],pos[1],pos[2]));
		model=translate(model,vec3(0,2,0));
		model=translate(model,vec3(startx,starty,startz));
		model=rotate(model,angle,vec3(0,1,0));
		model=scale(model,vec3(1.5,1.5,1.5));
		normalMatrix=transpose(inverse(view*model));
		glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
		glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
		buildRobot(); 
		model=modelStack.top();		
	}
};

//Creating a robot struct
Robot Robo;

//////////////////////////////////////////////////////////////////////////////////
//    GLUT stuff 
/////////////////////////////////////////////////////////////////////////////////

void mymouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		press_x = x; press_y = y; 
		if (button == GLUT_LEFT_BUTTON)
			xform_mode = XFORM_ROTATE; 
		else if (button == GLUT_RIGHT_BUTTON) 
			xform_mode = XFORM_SCALE; 
	}
	else if (state == GLUT_UP) {
		xform_mode = XFORM_NONE; 
	}
}
void mymotion(int x, int y)
{
	if (xform_mode==XFORM_ROTATE) {
		x_angle += (x - press_x)/5.0; 
		if (x_angle > 180) x_angle -= 360; 
		else if (x_angle <-180) x_angle += 360; 
		press_x = x; 

		y_angle += (y - press_y)/5.0; 
		if (y_angle > 180) y_angle -= 360; 
		else if (y_angle <-180) y_angle += 360; 
		press_y = y; 
	}
	else if (xform_mode == XFORM_SCALE){
		float old_size = scale_size;
		scale_size *= (1+ (y - press_y)/60.0); 
		if (scale_size <0) scale_size = old_size; 
		press_y = y; 
	}
	glutPostRedisplay(); 
}

void mykey(unsigned char key, int x, int y)
{
	if (key == 'q') 
		exit(1); 

	if(key=='f' )
	{
		movement=true;
		rotLArm=0;
		rotRArm=0;
		rotL=0;
		rotR=0;
		rotating=0;

		if(walk==0)
			walk=1;
		else
			walk=0;

		if(walk==0)
		{
			rotAngle=10;
			walkLen=0.2;
			walkLen1=-2.4;

		}
		else if(walk==1)
		{
			rotAngle=10;
			rotAngle1=10;
			walkLen=0.2;		
			walkLen1=-2.4;
		}

		Robo.moveForward(1);
	}

	if(key=='b')
	{
		rotating=0;
		movement=false;
		rotLArm=0;
		rotRArm=0;
		rotL=0;
		rotR=0;		

		if(moveBack==0)
			moveBack=1;
		else
			moveBack=0;

		if(moveBack==0)
		{
			rotAngle=10;
			walkLen=0.2;
			walkLen1=-2.4;
		}

		else if(moveBack==1)
		{
			rotAngle=10;
			rotAngle1=10;
			walkLen=0.2;	
			walkLen1=-2.4;
		}

		Robo.moveBackward(1);
	}


	if(key=='t')
	{
		walk=-1;
		moveBack=-1;

		turn=15;
		Robo.turnRight(turn);
	}

	if(key=='T')
	{	
		walk=-1;
		moveBack=-1;
		turn=15;
		Robo.turnLeft(turn);	
	}

	if(key=='l')
	{
		if(rotL==0)
			rotL=1;
		else
			rotL=0;

		rotLArm=15;		
		walk=-1;
		moveBack=-1;

	}

	if(key=='r')
	{
		if(rotR==0)
			rotR=1;
		else
			rotR=0;	

		rotRArm=15;	
		walk=-1;
		moveBack=-1;
	}

	if(key=='w')
	{
		if(polygonMode)
			polygonMode=false;
		else
			polygonMode=true;
	}

	if(key=='h')
	{
		if(rotHeadDir==0)
			rotHeadDir=1;
		else
			rotHeadDir=0;		
		if(rotHeadAngle<90)
			rotHeadAngle=rotHeadAngle+15;		
	}

	if(key=='H')
	{
		if(rotHeadDir==0)
			rotHeadDir=1;
		else
			rotHeadDir=0;

		if(rotHeadAngle>-90)
			rotHeadAngle=rotHeadAngle-15;	
	}

	if(key=='s')
	{
		if(shadeType==0)
		{
			shadeType=1;
			cout<<"Current Shading Model is Per Vertex Phong Shading or Gouraud Shading!"<<endl;
		}
		else if(shadeType==1)
		{
			shadeType=2;
			cout<<"Current Shading Model is  Toon or Cel Shading!"<<endl;
		}
		else if(shadeType==2)
		{
			shadeType=3;
			cout<<"Current Shading Model is Flat Shading!"<<endl;
		}
		else if(shadeType==3)
		{
			shadeType=0;
			cout<<"Current Shading Model is Per Fragment Phong Shading!"<<endl;
		}
	}

	if(key=='c')
	{
		for(int p=0;p<totObj-4;p++)
		{
			randomObjColor[p]=vec4 ( (float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,1);
		}

	}

	if(key=='o')
	{
		if(light1==0)
			light1=1;
		else
			light1=0;
	}

	if(key=='p')
	{
		if(light2==0)
			light2=1;
		else
			light2=0;
	}

	glutPostRedisplay();
}


/*
Deal with special key strokes
*/
void SpecialKey(int key,int x, int y)
{
	switch (key) 
	{
	case GLUT_KEY_LEFT:  Xcam = Xcam-1; break;
	case GLUT_KEY_RIGHT: Xcam = Xcam+1; break;
	case GLUT_KEY_UP:    Ycam = Ycam+1; break;
	case GLUT_KEY_DOWN:  Ycam = Ycam-1; break;
	}

	glutPostRedisplay(); 
}

//////////////////////////////////////////////////////////////////////////////////
// create VBO objects and send the triangle vertices/colors to the graphics card
// ///////////////////////////////////////////////////////////////////////////////

void InitVBO(int nslices, int nstacks) 
{
	int nvertices = nslices*nstacks; // same for sphere, cone and cylinder	
	int nindices = (nstacks-1)*2*(nslices+1); // same for sphere, cone and cylinder	
	int cubenvertices = 8;
	int cubenindices = 36;

	glGenBuffers(1, &vboHandle[0]);
	glGenBuffers(1, &indexVBO[0]);
	glGenBuffers(1, &vboHandle[1]);
	glGenBuffers(1, &indexVBO[1]);
	glGenBuffers(1, &vboHandle[2]);
	glGenBuffers(1, &indexVBO[2]);
	glGenBuffers(1, &vboHandle[3]);
	glGenBuffers(1, &indexVBO[3]);

	for(int i=4;i<totObj;i++)
	{
		glGenBuffers(1, &vboHandle[i]);
		glGenBuffers(1, &indexVBO[i]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);   // bind the first handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, spverts, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, spindices, GL_STATIC_DRAW);  // load the index data 
	glBindBuffer(GL_ARRAY_BUFFER,0);  // clean up 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up

	//Initialize Cylinder VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);  // bind the second handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, cyverts, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexVBO[1]); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, cyindices, GL_STATIC_DRAW);  // load the index data 
	glBindBuffer(GL_ARRAY_BUFFER,0);  // clean up 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up

	//Initialize Cone VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);  // bind the third handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, cnverts, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexVBO[2]); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, cnindices, GL_STATIC_DRAW);  // load the index data 
	glBindBuffer(GL_ARRAY_BUFFER,0);  // clean up 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up	

	//Initialize Cube VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[3]);   // bind the fourth handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*cubenvertices, cbverts, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*cubenindices, cbindices, GL_STATIC_DRAW);  // load the index data 
	glBindBuffer(GL_ARRAY_BUFFER,0);  // clean up 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up

	//Initialize the Ply object VBO
	for(int i=4;i<totObj;i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboHandle[i]);   // bind the fifth handle
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexcount[i-4], plyverts[i-4], GL_STATIC_DRAW); // allocate space and copy the position data over
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*facecount[i-4]*3, plyindices[i-4], GL_STATIC_DRAW);  // load the index data 
		glBindBuffer(GL_ARRAY_BUFFER,0);  // clean up 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up
	}

	// by now, we moved the position and color data over to the graphics card. There will be no redundant data copy at drawing time 	
}

////////////////////////////////////////////////////////////////////////////////////

void InitCylinder(int nslices, int nstacks,int baseRadius, int topradius,int height) 
{
	float factor=0;
	int top_radius=topradius;
	int base_radius=baseRadius;

	int nvertices = nslices * nstacks; 
	cyverts = new Vertex[nvertices]; 	

	float Dangle = 2*PI/(float)(nslices-1); 

	for (int j =0; j<nstacks; j++)
	{
		if(base_radius==top_radius)
			factor=1;
		else
			factor = base_radius - j*((base_radius-top_radius)/(float)(nstacks-1));

		for (int i=0; i<nslices; i++) 
		{
			int idx = j*nslices + i; // mesh[j][i] 
			float angle = Dangle * i; 

			//set vertices
			cyverts[idx].location[0] = factor*cos(angle); 
			cyverts[idx].location[1] = factor*sin(angle);
			cyverts[idx].location[2] = j*height/(float)(nstacks-1); 
			cyverts[idx].location[3] = 1.0; 
			//set normals
			cyverts[idx].normal[0] = cos(angle);			 
			cyverts[idx].normal[1] = sin(angle);			
			cyverts[idx].normal[2] = 0.0; 
			cyverts[idx].normal[3] = 0.0;

			cyverts[idx].color[0] = 0.5;			 
			cyverts[idx].color[1] = 0;			
			cyverts[idx].color[2] = 0.5; 
			cyverts[idx].color[3] = 1.0;

		}
	}
	// now create the index array 

	cylinderindices = (nstacks-1)*2*(nslices+1); 
	cyindices = new GLuint[cylinderindices]; 
	int n = 0; 
	for (int j =0; j<nstacks-1; j++)
		for (int i=0; i<=nslices; i++) 
		{
			int mi = i % nslices;  
			//cout<<i<<"::"<<mi<<endl;
			int idx = j*nslices + mi; // mesh[j][mi] 
			int idx2 = (j+1) * nslices + mi; 
			cyindices[n++] = idx; 
			cyindices[n++] = idx2; 
		}
}

////////////////////////////////////////////////////////////////////////////////////

void InitCone(int nslices, int nstacks, int baseRadius, int height) 
{
	int nvertices = nslices * nstacks; 
	cnverts = new Vertex[nvertices]; 
	float factor=0;
	int top_radius=0;
	int base_radius=baseRadius;

	float Dangle = 2*PI/(float)(nslices-1); 

	for (int j = 0; j < nstacks; j++)
	{
		factor = (nstacks-j-1)*((base_radius-top_radius)/(float)nstacks);

		for (int i=0; i<nslices; i++) 
		{
			int idx = j*nslices + i; // mesh[j][i] 
			float angle = Dangle * i; 
			cnverts[idx].location[0] = factor*cos(angle);  
			cnverts[idx].normal[0] = cos(angle);
			cnverts[idx].location[1] = factor*sin(angle);  
			cnverts[idx].normal[1] = sin(angle);
			cnverts[idx].location[2] = j*height/(float)(nstacks-1); 
			cnverts[idx].normal[2] = 0.0; 
			cnverts[idx].location[3] = 1.0;  
			cnverts[idx].normal[3] = 0.0; 

			cnverts[idx].color[0] = 0.5;			 
			cnverts[idx].color[1] = 0.5;			
			cnverts[idx].color[2] = 0.5; 
			cnverts[idx].color[3] = 1.0;
		}
	}

	// now create the index array 
	coneindices = (nstacks-1)*2*(nslices+1); 
	cnindices = new GLuint[coneindices]; 
	int n = 0; 
	for (int j =0; j<nstacks-1; j++)
		for (int i=0; i<=nslices; i++) 
		{
			int mi = i % nslices;  
			int idx = j*nslices + mi; // mesh[j][mi] 
			int idx2 = (j+1) * nslices + mi; 
			cnindices[n++] = idx; 
			cnindices[n++] = idx2; 
		}
}

////////////////////////////////////////////////////////////////////////////////////

void InitSphere(int nslices,int nstacks,int radius)
{
	int i,j;
	double theta1,theta2;

	//make nslices even
	nslices/=2;
	nslices*=2;

	int nvertices = nslices*nstacks;	
	spverts = new Vertex[nvertices]; 	

	for(i=0;i<nslices;i++)
	{
		spverts[i].location[0]=0;
		spverts[i].location[1]=-radius;
		spverts[i].location[2]=0;
		spverts[i].location[3]=1;

		spverts[i].normal[0] = 0;
		spverts[i].normal[1] = -1;
		spverts[i].normal[2] = 0;
		spverts[i].normal[3] = 1; 

		spverts[i].color[0] = 1;
		spverts[i].color[1] = 0;
		spverts[i].color[2] = 0;
		spverts[i].color[3] = 1; 

	}

	int n=nslices;
	for (j=1;j<nstacks-1;j++) 
	{
		theta1 = j * 2*PI /nslices - PI/2; 

		for (i=0;i<nslices;i++) 
		{
			theta2 = i *2*PI / nslices;

			spverts[n].location[0] = radius*cos(theta1) * cos(theta2);
			spverts[n].location[1] = radius*sin(theta1);
			spverts[n].location[2] = radius*cos(theta1) * sin(theta2);
			spverts[n].location[3] = 1; 			

			spverts[n].normal[0] = cos(theta1) * cos(theta2);
			spverts[n].normal[1] = sin(theta1);
			spverts[n].normal[2] = cos(theta1) * sin(theta2);
			spverts[n].normal[3] = 1; 

			spverts[n].color[0] = 1;
			spverts[n].color[1] = 0;
			spverts[n].color[2] = 0;
			spverts[n].color[3] = 1; 

			n++;			
		}		
	}

	for(i=0;i<nslices;i++)
	{
		spverts[n].location[0]=0;
		spverts[n].location[1]=radius;
		spverts[n].location[2]=0;
		spverts[n].location[3]=1;

		spverts[n].normal[0] = 0;
		spverts[n].normal[1] = 1;
		spverts[n].normal[2] = 0;
		spverts[n].normal[3] = 1; 

		spverts[n].color[0] = 1;
		spverts[n].color[1] = 0;
		spverts[n].color[2] = 0;
		spverts[n].color[3] = 1; 

		n++;
	}

	sphereindices = (nstacks-1)*2*(nslices+1); 
	spindices = new GLuint[sphereindices]; 

	n = 0; 		
	for (int j =0; j<nstacks-1; j++)
		for (int i=0; i<=nslices; i++) 
		{
			int mi = i % nslices;  
			int idx = j*nslices + mi; // mesh[j][mi] 
			int idx2 = (j+1) * nslices + mi; 
			spindices[n++] = idx;			
			spindices[n++] = idx2; 			
		}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitCube(int size)
{
	int nvertices = 8;
	cbverts = new Vertex[nvertices];
	cubeindices = 36;
	cbindices = new GLuint[cubeindices];

	cbverts[0].location[0] = -(float)size/2;    cbverts[4].location[0] = -(float)size/2;
	cbverts[0].location[1] = -(float)size/2;    cbverts[4].location[1] = (float)size/2;
	cbverts[0].location[2] = (float)size/2;     cbverts[4].location[2] = -(float)size/2;
	cbverts[0].location[3] =1;                  cbverts[4].location[3] =1;

	cbverts[1].location[0] = (float)size/2;     cbverts[5].location[0] = (float)size/2;
	cbverts[1].location[1] = -(float)size/2;    cbverts[5].location[1] = (float)size/2;
	cbverts[1].location[2] = (float)size/2;     cbverts[5].location[2] = -(float)size/2;
	cbverts[1].location[3] =1;                  cbverts[5].location[3] =1;

	cbverts[2].location[0] = -(float)size/2;    cbverts[6].location[0] = -(float)size/2;
	cbverts[2].location[1] = (float)size/2;     cbverts[6].location[1] = -(float)size/2;
	cbverts[2].location[2] = (float)size/2;     cbverts[6].location[2] = -(float)size/2;
	cbverts[2].location[3] =1;                   cbverts[6].location[3] =1;

	cbverts[3].location[0] = (float)size/2;     cbverts[7].location[0] = (float)size/2;
	cbverts[3].location[1] = (float)size/2;     cbverts[7].location[1] = -(float)size/2;
	cbverts[3].location[2] = (float)size/2;     cbverts[7].location[2] = -(float)size/2;
	cbverts[3].location[3] =1;                  cbverts[7].location[3] =1;


	cbverts[0].normal[0] = -0.5;    cbverts[4].normal[0] = -0.5;
	cbverts[0].normal[1] = -0.5;    cbverts[4].normal[1] = 0.5;
	cbverts[0].normal[2] = 0.5;     cbverts[4].normal[2] = -0.5;
	cbverts[0].normal[3] =1;        cbverts[4].normal[3] =1;

	cbverts[1].normal[0] = 0.5;     cbverts[5].normal[0] = 0.5;
	cbverts[1].normal[1] = -0.5;    cbverts[5].normal[1] = 0.5;
	cbverts[1].normal[2] = 0.5;     cbverts[5].normal[2] = -0.5;
	cbverts[1].normal[3] =1;        cbverts[5].normal[3] =1;

	cbverts[2].normal[0] = -0.5;    cbverts[6].normal[0] = -0.5;
	cbverts[2].normal[1] = 0.5;     cbverts[6].normal[1] = -0.5;
	cbverts[2].normal[2] = 0.5;     cbverts[6].normal[2] = -0.5;
	cbverts[2].normal[3] =1;        cbverts[6].normal[3] =1;

	cbverts[3].normal[0] = 0.5;     cbverts[7].normal[0] = 0.5;
	cbverts[3].normal[1] = 0.5;     cbverts[7].normal[1] = -0.5;
	cbverts[3].normal[2] = 0.5;     cbverts[7].normal[2] = -0.5;
	cbverts[3].normal[3] =1;        cbverts[7].normal[3] =1;

	cbindices[0]=0;     cbindices[1]=1;    cbindices[2]=2; 
	cbindices[3]=2;     cbindices[4]=1;    cbindices[5]=3;
	cbindices[6]=2;     cbindices[7]=3;    cbindices[8]=4;
	cbindices[9]=4;     cbindices[10]=3;  cbindices[11]=5; 
	cbindices[12]=4;     cbindices[13]=5;  cbindices[14]=6;
	cbindices[15]=5;     cbindices[16]=6;  cbindices[17]=7;
	cbindices[18]=0;     cbindices[19]=1;  cbindices[20]=6;
	cbindices[21]=7;     cbindices[22]=6;  cbindices[23]=1;
	cbindices[24]=2;     cbindices[25]=6;  cbindices[26]=4;
	cbindices[27]=1;     cbindices[28]=7;  cbindices[29]=3;
	cbindices[30]=3;     cbindices[31]=7;  cbindices[32]=5;
	cbindices[33]=2;     cbindices[34]=6;  cbindices[35]=0;

	cbverts[0].color[0] =0.5;      
	cbverts[0].color[1] =0.5;
	cbverts[0].color[2] =0.5;
	cbverts[0].color[3] =1;

	cbverts[1].color[0] =0.5;      
	cbverts[1].color[1] =0.5;
	cbverts[1].color[2] =0.5;
	cbverts[1].color[3] =1;

	cbverts[2].color[0] =0.5;      
	cbverts[2].color[1] =0.5;
	cbverts[2].color[2] =0.5;
	cbverts[2].color[3] =1;

	cbverts[3].color[0] =0.5;      
	cbverts[3].color[1] =0.5;
	cbverts[3].color[2] =0.5;
	cbverts[3].color[3] =1;

	cbverts[4].color[0] =0.5;      
	cbverts[4].color[1] =0.5;
	cbverts[4].color[2] =0.5;
	cbverts[4].color[3] =1;

	cbverts[5].color[0] =0.5;      
	cbverts[5].color[1] =0.5;
	cbverts[5].color[2] =0.5;
	cbverts[5].color[3] =1;

	cbverts[6].color[0] =0.5;      
	cbverts[6].color[1] =0.5;
	cbverts[6].color[2] =0.5;
	cbverts[6].color[3] =1;

	cbverts[7].color[0] =0.5;      
	cbverts[7].color[1] =0.5;
	cbverts[7].color[2] =0.5;
	cbverts[7].color[3] =1;
}

void InitPly(char** argv)
{
	//create my own array for handling ply data and passing to VBO
	randomObjColor = new vec4[totObj];
	plyObjPos = new vec3[2]; // loads only 2 ply files at the same time, 1 Motorbike, 2 : vishnu
	plyObjScale = new vec3[2];
	plyObjRotate = new float[2];

	plyObjRotateAxis = new vec3[2];

    plyObjPos[0] = vec3(25,-15,20); // bike
	plyObjPos[1] = vec3(-40,20,0); // vishnu

	plyObjScale[0] = vec3(0.02,0.02,0.02);
	plyObjScale[1] = vec3(0.09,0.09,0.09);

	plyObjRotateAxis[0] = vec3(0,1,0);
	plyObjRotateAxis[1] = vec3(1,0,0);

	plyObjRotate[0] = 45.0f;
	plyObjRotate[1] = -90.0f;

	for(int p=0;p<totObj-4;p++)
	{
		plyverts[p] = new Vertex[vertexcount[p]];
		plyindices[p] = new GLuint[facecount[p]*3];
		int i,j; 	
		float x,y,z;
		float x0,y0,z0;
		float x1,y1,z1;
		float len;
		float recip;

		randomObjColor[p]=vec4 ( (float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,1);
		
		/* find normal of each face and add it to each vertex adjacent to the face */

		if(!has_normal[p]) // If ply file do not has normal then compute per vertex normals
		{
			for (i = 0; i < facecount[p]; i++) 
			{
				/* determine vectors parallel to two edges of face */
				x0 = vertices[p][faces[p][i]->vertices[faces[p][i]->count-1]]->x - vertices[p][faces[p][i]->vertices[0]]->x;
				y0 = vertices[p][faces[p][i]->vertices[faces[p][i]->count-1]]->y - vertices[p][faces[p][i]->vertices[0]]->y;
				z0 = vertices[p][faces[p][i]->vertices[faces[p][i]->count-1]]->z - vertices[p][faces[p][i]->vertices[0]]->z;   

				x1 = vertices[p][faces[p][i]->vertices[1]]->x - vertices[p][faces[p][i]->vertices[0]]->x;
				y1 = vertices[p][faces[p][i]->vertices[1]]->y - vertices[p][faces[p][i]->vertices[0]]->y;
				z1 = vertices[p][faces[p][i]->vertices[1]]->z - vertices[p][faces[p][i]->vertices[0]]->z;

				/* find cross-product between these vectors */
				x = y0 * z1 - z0 * y1;
				y = z0 * x1 - x0 * z1;
				z = x0 * y1 - y0 * x1;

				/* normalize this vector */
				len = x*x + y*y + z*z;
				if (len == 0) 
				{
					x = y = z = 0;
				}
				else {
					recip = -1 / std::sqrt (len);

					x *= recip;
					y *= recip;
					z *= recip;
				}

				/* add this normal to each vertex that is adjacent to face */
				for (j = 0; j < faces[p][i]->count; j++) 
				{
					vertices[p][faces[p][i]->vertices[j]]->nx = x;
					vertices[p][faces[p][i]->vertices[j]]->ny = y;
					vertices[p][faces[p][i]->vertices[j]]->nz = z;      
				}
			}

			/* normalize all the normals at the vertices */
			for (i = 0; i < vertexcount[p]; i++) 
			{

				len = vertices[p][i]->nx * vertices[p][i]->nx + vertices[p][i]->ny * vertices[p][i]->ny + vertices[p][i]->nz * vertices[p][i]->nz;
				if (len == 0) 
				{
					vertices[p][i]->nx= 0;
					vertices[p][i]->ny = 0;
					vertices[p][i]->nz = 0;
				}
				else 
				{      
					recip = 1 / std::sqrt (len);
					vertices[p][i]->nx *= recip;
					vertices[p][i]->ny *= recip;
					vertices[p][i]->nz *= recip;
				}

			}
		}

		for(int i = 0; i < vertexcount[p]; i++) 
		{
			plyverts[p][i].location[0]=vertices[p][i]->x;
			plyverts[p][i].location[1]=vertices[p][i]->y;
			plyverts[p][i].location[2]=vertices[p][i]->z;
			plyverts[p][i].location[3]=1;

			if(strcmp(argv[p+1], "bunny.ply") == 0) // flip normals for bunny
			{

				plyverts[p][i].normal[0]=-vertices[p][i]->nx; 
				plyverts[p][i].normal[1]=-vertices[p][i]->ny;
				plyverts[p][i].normal[2]=-vertices[p][i]->nz;
				plyverts[p][i].normal[3]=1;

			}

			else
			{
				plyverts[p][i].normal[0]=vertices[p][i]->nx; 
				plyverts[p][i].normal[1]=vertices[p][i]->ny;
				plyverts[p][i].normal[2]=vertices[p][i]->nz;
				plyverts[p][i].normal[3]=1;

			}

			plyverts[p][i].color[0]=0.80;
			plyverts[p][i].color[1]=0.70;
			plyverts[p][i].color[2]=0.21;
			plyverts[p][i].color[3]=1;
		}

		int n=0;
		//Load the indices information
		for(int i=0;i<facecount[p];i++)
		{
			plyindices[p][n++] = faces[p][i]->vertices[0];
			plyindices[p][n++] = faces[p][i]->vertices[1];
			plyindices[p][n++] = faces[p][i]->vertices[2];
		}

		n=0;
	}

}

/////////////////////////////////////////////////////////////////////////////////
void draw_cube()
{	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[3]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[3]);
	glEnableVertexAttribArray(c0); 
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glDrawElements(GL_TRIANGLES,cubeindices,GL_UNSIGNED_INT,(char *) NULL+0);
}


void draw_cylinder()
{	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[1]);
	glEnableVertexAttribArray(c0); 
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glDrawElements(GL_TRIANGLE_STRIP,cylinderindices,GL_UNSIGNED_INT,(char *) NULL+0);
}


void draw_sphere()
{	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glEnableVertexAttribArray(c0); 
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
}

void draw_cone()
{	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[2]);
	glEnableVertexAttribArray(c0); 
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glDrawElements(GL_TRIANGLE_STRIP,coneindices,GL_UNSIGNED_INT,(char *) NULL+0);
}

void draw_ply(int i)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[i+4]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[i+4]);
	glEnableVertexAttribArray(c0); 
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glDrawElements(GL_TRIANGLES,facecount[i]*3,GL_UNSIGNED_INT,(char *) NULL+0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawModel()
{
	for(int i=0;i<totObj-4;i++)
	{
		modelStack.push(model);
		//model=translate(model,vec3(30*i*pow(-1.0,i),-3*i,8*pow(-1.0,i)));
		model = translate(model,vec3(plyObjPos[i]));
		model=scale(model,vec3(plyObjScale[i]));
		model=rotate(model,plyObjRotate[i],plyObjRotateAxis[i]);
		normalMatrix=transpose(inverse(view*model));
		diffuseTerm=randomObjColor[i];		
		glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
		glUniform4fv(m5, 1,&diffuseTerm[0]);
		draw_ply(i);
		model=modelStack.top();
	}
}

void drawStaticScene()
{
	///////////////Draw base cube//////////////////
	modelStack.push(model);	
	model=translate(model,vec3(0,-4,0));
	model=scale(model,vec3(160,2,160));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(1,0.87,0.67,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();	

	///////////////Draw Pillers//////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-30,-3,-60));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(-30,-3,-30));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(-20,-3,-60));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(-20,-3,-30));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(-10,-3,-60));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(-10,-3,-30));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(0,-3,-60));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(0,-3,-30));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(10,-3,-60));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(10,-3,-30));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(20,-3,-60));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(20,-3,-30));
	model=scale(model,vec3(1.5,25,1));
	model=rotate(model,-90.0f,vec3(1,0,0));
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.8,0.5,0.2,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder();	
	model=modelStack.top();

	//Pillers end here

	//Draw Roof Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,23,-45));
	model=scale(model,vec3(70,2,34));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.5,0.5,0.5,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();	


	//Draw slant front roof Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,33,-37));
	model = rotate(model,45.0f,vec3(1,0,0));
	model=scale(model,vec3(70,2,25));  
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.5,0.5,0.5,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); // lower base of manson
	model=modelStack.top();	


	//Draw slant back roof Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,33,-54));
	model = rotate(model,-45.0f,vec3(1,0,0));
	model=scale(model,vec3(70,2,25));  
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.5,0.5,0.5,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); // lower base of manson
	model=modelStack.top();

	//Draw base 1 Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,-2,-45));
	model=scale(model,vec3(80,2,38));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.5,0.5,0.5,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();

	//Draw base 2 Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,0,-45));
	model=scale(model,vec3(70,2,36));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.5,0.5,0.5,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();

	//Draw base 3 Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,2,-45));
	model=scale(model,vec3(60,2,34));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.5,0.5,0.5,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();

}

///////////////////////////////////////////////////////////////////////////////
void display() 
{ 
	glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	if(shadeType==0)
	{
		programObject = perFragPhong;	
	}
	else if(shadeType==1)
	{
		programObject = perVertexPhong;	
	}
	else if(shadeType==2)
	{
		programObject=toonShader;	
	}
	else
		programObject=flat;

	glUseProgram(programObject);	

	// get the location of the uniform variable in the shader
	c0 = glGetAttribLocation(programObject, "position"); 
	c1 = glGetAttribLocation(programObject, "normal"); 
	c2 = glGetAttribLocation(programObject, "color1");
	m1 = glGetUniformLocation(programObject, "projection"); 
	m2 = glGetUniformLocation(programObject, "model");
	m3 = glGetUniformLocation(programObject, "view");
	m4 = glGetUniformLocation(programObject, "normalMatrix");
	m5 = glGetUniformLocation(programObject, "diffuseTerm");
	m6 = glGetUniformLocation(programObject, "kamb");
    m7 = glGetUniformLocation(programObject, "kdiff");
    m8 = glGetUniformLocation(programObject, "kspec");
	m9 = glGetUniformLocation(programObject, "shininess");
	l1On = glGetUniformLocation(programObject, "L1");
	l2On = glGetUniformLocation(programObject, "L2");
	

	if(polygonMode)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // comment this line to enable polygon shades
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); 

	model = mat4(1.0f);	
	mat4 projection = perspective(75.0f,1.0f,0.1f,1000.0f); 
	view = lookAt(vec3(-30.0, 40.0, 95.0),vec3(Xcam,Ycam,Zcam),vec3(0.0, 1.0, 0.0));  

	model =rotate(model, x_angle, vec3(0.0f, 1.0f, 0.0f));   
	model = rotate(model, y_angle, vec3(1.0f, 0.0f, 0.0f));   
	model = scale(model, vec3(scale_size, scale_size, scale_size));  

	normalMatrix=transpose(inverse(view*model));// normalMatrix is inverse transpose of modelView matrix

	// construct the modelview and modelview projection matrices 	 
	glUniformMatrix4fv(m1, 1, GL_FALSE, &projection[0][0]); // pass the viewProjection matrix to the shader as a uniform 
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]); // pass the model matrix to the shader as a uniform 
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);  // pass view matrix as uniform to shader
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]); // pass normal matrix (inverse transpose of modelView matrix as uniform to shader
	glUniform4fv(m5,1,&diffuseTerm[0]); 
	glUniform1f(m6,kamb);
	glUniform1f(m7,kdiff);
	glUniform1f(m8,kspec);
    glUniform1i(m9,shininess);
	glUniform1i(l1On,light1);
	glUniform1i(l2On,light2);


	//******************************************************//
	/********Do all drawing stuffs here*********************/
	drawModel(); // draws ply objects	

	//Create Static Scene			
	drawStaticScene();	// static scene is always drawn using phong shading


	//Dynamic Object transformations start here for moving the blobby man
	Robo.draw(0,0,30); //( Initial position of the robot (0,0,0) this can be changed    

	glutSwapBuffers(); 
	glUseProgram(0);
}

void reshape(int w, int h)
{
	width=w;
	height=h;
	glViewport(0,0,w,h);
}

/////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) 
{

	/* initialize random seed: */
  	srand (time(NULL));

	if(argc==1)
	{
		cout<<"No data file is loaded"<<endl;
		system("pause");
		exit(0);
	}

	totObj=(argc-1)+4;

	std::cout<<totObj<<endl;

	PlyFile* input[20];
	
	for(int i=0;i<totObj-4;i++)
	{
		// get the ply structure and open the file
		FILE* fp1 = fopen(argv[i+1],"r");
		//input = read_ply(stdin);
		input[i] = read_ply(fp1);
		// read in the data
		store_ply(input[i],&vertices[i], &faces[i],&vertexcount[i], &facecount[i],&vertexnormals[i], &facenormals[i], &has_normal[i]);
		// close the file
		close_ply(input[i]);
		//fclose(fp1); // this line causing the double free crash issue?? Why?
	}

	/*find_center(cx, cy, cz, x_min, x_max,y_min, y_max, z_min, z_max); 
	printf("geometry center = %f %f %f \n", cx, cy, cz); 
	printf("geometry bound = x: %f %f y: %f %f z: %f %f\n",x_min, x_max, y_min, y_max, z_min, z_max);*/

	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
	glutInitWindowSize(width,height); 

	glutCreateWindow("Blobby Man Walking ;-)"); 
	glutDisplayFunc(display); 
	glutMouseFunc(mymouse); 
	glutMotionFunc(mymotion);
	glutKeyboardFunc(mykey); 
	glutSpecialFunc(SpecialKey);
	glutReshapeFunc(reshape);

	// initialize GLEW 
	GLenum err = glewInit(); 

	if ( err != GLEW_OK) 
		cout<<" Error initializing GLEW! \n"<<endl; 
	else cout<<"Initializing GLEW succeeded!\n"<<endl; 

	// define the discretion level for the cylinder 
	int nslices, nstacks; 
	nslices = 40;  
	nstacks = nslices/2+1; 

	perFragPhong = SetupGLSL("perFragPhong");  //create perFragPhong shaders
	toonShader = SetupGLSL("toonShader"); // create toon shader
	perVertexPhong = SetupGLSL("perVertexPhong"); // create perVertexPhong shader
	flat = SetupGLSL("flat"); // create perVertexPhong shader

	InitCylinder(nslices, nstacks,1,1,1); 
	InitCone(nslices,nstacks,1,1); 	
	InitSphere(nslices,nstacks,1);
	InitCube(1);
	InitPly(argv);

	InitVBO(nslices, nstacks); 
	Robo.angle=0;

	glutMainLoop(); 
} 