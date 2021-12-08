///////////////////////////////////////////////////////////////////////
//          CSE 5542 Course                                          //
//          Lab4                                                     //
//          Implemented by: Soumya Dutta                             //
//          Graduate Student                                         //
//          duttas@cse.ohio-state.edu                                //
//          GLSL Advanced Featurs Implementation                     //
///////////////////////////////////////////////////////////////////////

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
#include <cstdlib>
#include "ply.h"

using namespace glm;
using namespace std;

const float M_PI=3.14f;

//Ply stuff
typedef struct VertexPly 
{
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
	float texCoord[2];
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
GLuint c0,c1,c2,c3,p0,p1;
float rot=0;
mat4 XX = mat4(1.0f);
mat4 normalMatrix = mat4(1.0);
mat4 worldNormalMatrix = mat4(1.0);
mat4 lightViewMatrix = mat4(1.0);
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
GLuint texLoc=0;
GLuint cmap=0;
GLuint wnm=0; //world normal matrix
GLuint cubereq=0;
GLuint bumpreq=0;
GLuint normalTex=0;
vec4 diffuseTerm;
GLuint programObject;
GLuint perFragPhong;
GLuint cubeMap;
GLuint bumpMap;
GLuint shadowMap;
GLuint epos;
int shadeType=3;
vec3 COI = vec3(0,80,0);
float kspec=0.7; float kdiff=0.6; float kamb=0.2;
int shininess =50;
int light1=1;
int light2=1;
GLuint tid[20],cubemap_texture;
int needTexture=1;
int needtex =0;
GLubyte *base,*back,*marbleTexture,*piller,*tiles,*leftIm,*rightIm,*roof, *bumpNormal, *bumpTexture;
GLubyte *cubeMapImages[6];
vec3 eyePos = vec3(0.0,200.0,250.0);
int isCubeMap=0, isBumpMap=0, isShadow=0;
GLuint fboHandle[5]; 
GLuint depthTex; 
GLuint shadowTex=0;
GLuint lightView=0;
GLuint reflectionTex=0;
GLuint depthBuf=0;
GLuint reflectTex=0;
GLuint isReflect=0;
GLuint reflectFlag=0;
GLuint m10=0;
mat4 invMVP = mat4(1.0f);
mat4 prevPVMat = mat4(1.0f);

GLubyte* read_Image(char* filename) 
{
	GLubyte *texImage1;
	GLubyte *readImage;

	int r, g, b; 	 
	int height, width, maxVal; 
	char buff[16]; 
	char c;

	FILE* in = fopen(filename, "rb");
	if (!in) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		system("pause");
	}

	//read image format
	if (!fgets(buff, sizeof(buff), in)) 
	{
		fprintf(stderr,"can't read header of image file");
		system("pause");              
	}


	//check for comments
	c = getc(in);
	while (c == '#') 
	{
		while (getc(in) != '\n') ;
		c = getc(in);
	}

	ungetc(c,in);

	fscanf(in, "%d %d %d", &width, &height, &maxVal); 

	texImage1 = (GLubyte  *)malloc((height*width*4)*sizeof(GLubyte));
	readImage = (GLubyte  *)malloc((height*width*4)*sizeof(GLubyte));

	int count = 0;
	for (int i=height-1; i>=0; i--)
		for (int j=0; j<width; j++)
		{
			fscanf(in, "%d %d %d", &r, &g, &b); 
			readImage[count] = (GLubyte)r; 
			readImage[count+1] = (GLubyte)g; 
			readImage[count+2] = (GLubyte)b; 
			readImage[count+3] = 255;
			count += 4;
		}

		count = 0;
		for (int i=0; i<width; i++)
			for ( int j=0; j<height; j++)
			{
				if (i<height && j <width) 
				{
					texImage1[count] = readImage[count];
					texImage1[count+1] = readImage[count+1];
					texImage1[count+2] = readImage[count+2];
					texImage1[count+3] = 255;
					count += 4;
				}
				else 
				{
					texImage1[count] = 0;
					texImage1[count+1] = 0;
					texImage1[count+2] = 0;
					texImage1[count+3] = 1;
					count += 4;
				}
			}

			fclose(in); 
			return texImage1;
}

//////////////////////////////////
int totObj; // totObj = number of ply files read
vec4 *randomObjColor;
vec3 *plyObjPos;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void store_ply(PlyFile* input, VertexPly ***vertices, Face ***faces,unsigned int* vertexcount, 
	unsigned int* facecount,int* vertexnormals, int* facenormals, bool* has_normal) 
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
		vec3 forward = vec3(sin(M_PI*angle/180), 0, cos(M_PI*angle/180));	
		pos += forward * dist;			
	}

	void moveBackward(float dist)
	{
		vec3 original_forward = vec3(0,0,1);
		vec3 forward = vec3(sin(M_PI*angle/180), 0, cos(M_PI*angle/180));
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
		model=translate(model,vec3(0,5,0));
		model=translate(model,vec3(startx,starty,startz));
		model=rotate(model,angle,vec3(0,1,0));
		model=scale(model,vec3(2.5,2.5,2.5));
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

		turn=5;
		Robo.turnRight(turn);
	}

	if(key=='T')
	{	
		walk=-1;
		moveBack=-1;
		turn=5;
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
			cout<<"Current Shading Model is Per Fragment Phong Shading with cubeMap!"<<endl;
		}		
		else if(shadeType==1)
		{
			shadeType=2;			
			cout<<"Current Shading Model is Per Fragment Phong Shading with bumpMap!"<<endl;
		}
		else if(shadeType==2)
		{
			shadeType=3;			
			cout<<"Current Shading Model is Per Fragment Phong Shading with Shadow!"<<endl;
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
	case GLUT_KEY_LEFT:  COI[0] -= 5; break;
	case GLUT_KEY_RIGHT: COI[0] += 5; break;
	case GLUT_KEY_UP:    COI[1] -= 5; break;
	case GLUT_KEY_DOWN:  COI[1] += 5; break;
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

	//Load Cube Texture to GPU
	glGenTextures(1, &cubemap_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	for (int i=0; i<6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0,GL_RGBA, 512,512,0, GL_RGBA, GL_UNSIGNED_BYTE,	cubeMapImages[i]); // assume cubeMapImages[i] has already been read in
	}


	//Load image Textures to GPU
	glGenTextures(1,&tid[0]);
	glBindTexture(GL_TEXTURE_2D,tid[0]);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,base);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);


	glGenTextures(1,&tid[1]);
	glBindTexture(GL_TEXTURE_2D,tid[1]);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,back);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );

	glGenTextures(1,&tid[2]);
	glBindTexture(GL_TEXTURE_2D,tid[2]);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,leftIm);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );

	glGenTextures(1,&tid[3]);
	glBindTexture(GL_TEXTURE_2D,tid[3]);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,rightIm);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );

	glGenTextures(1,&tid[4]);
	glBindTexture(GL_TEXTURE_2D,tid[4]);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,roof);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );

	glGenTextures(1,&tid[5]);
	glBindTexture(GL_TEXTURE_2D,tid[5]);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,marbleTexture);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );


	glGenTextures(1,&tid[6]);
	glBindTexture(GL_TEXTURE_2D,tid[6]);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,piller);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );

	glGenTextures(1,&tid[7]);
	glBindTexture(GL_TEXTURE_2D,tid[7]);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,tiles);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );

	glGenTextures(1,&tid[8]);
	glBindTexture(GL_TEXTURE_2D,tid[8]);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,512,512,0,GL_RGBA,GL_UNSIGNED_BYTE,bumpNormal);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );

	// by now, we moved the position and color data over to the graphics card. There will be no redundant data copy at drawing time 	
}

void InitFBO() 
{
	//FBO For Shadow
	glGenFramebuffers(1, &fboHandle[0]); 
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle[0]); 

	glGenTextures(1, &depthTex); 
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096,4096, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
	//GLfloat border[] = {0.0f, 0.0f, 0.0f, 0.0f};
	//glTexParameterfv (GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC, GL_LESS); 

	glDrawBuffer(GL_NONE); 
	glReadBuffer(GL_NONE); 

	//FBO for Reflection
	glGenFramebuffers(1, &fboHandle[1]); 
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle[1]); 

	glGenTextures(1, &reflectionTex); 
	glBindTexture(GL_TEXTURE_2D, reflectionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800,800, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTex, 0);

	/*glGenRenderbuffers(1, &depthBuf); 
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf); 
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800,800);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, depthBuf); 
	glDrawBuffer(GL_COLOR_ATTACHMENT0);	*/

	// check FBO status
	GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
		printf("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO\n");

	glBindFramebuffer(GL_FRAMEBUFFER,0); // go back to the default framebuffer 

}

////////////////////////////////////////////////////////////////////////////////////

void InitCylinder(int nslices, int nstacks,int baseRadius, int topradius,int height) 
{
	float factor=0;
	int top_radius=topradius;
	int base_radius=baseRadius;

	int nvertices = nslices * nstacks; 
	cyverts = new Vertex[nvertices]; 	

	float Dangle = 2*M_PI/(float)(nslices-1); 

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

			cyverts[idx].texCoord[0]= acos(cyverts[idx].location[0])/2*M_PI/2;
			cyverts[idx].texCoord[1]= cyverts[idx].location[2];
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

	float Dangle = 2*M_PI/(float)(nslices-1); 

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


			cnverts[idx].texCoord[0]= acos(cnverts[idx].location[0])/2*M_PI;
			cnverts[idx].texCoord[1]=cnverts[idx].location[2];

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

		spverts[i].texCoord[0]= atan2(spverts[i].normal[0],spverts[i].normal[2]) / (2. * M_PI) + 0.5;
		spverts[i].texCoord[1]= asin(spverts[i].normal[1]) / M_PI + .5;
	}

	int n=nslices;
	for (j=1;j<nstacks-1;j++) 
	{
		theta1 = j * 2*M_PI /nslices - M_PI/2; 

		for (i=0;i<nslices;i++) 
		{
			theta2 = i *2*M_PI / nslices;

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

			spverts[n].texCoord[0]= atan2(spverts[n].normal[0],spverts[n].normal[2]) / (2. * M_PI) + 0.5;
			spverts[n].texCoord[1]= asin(spverts[n].normal[1]) / M_PI + 0.5;	

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

		spverts[n].texCoord[0]= atan2(spverts[n].normal[0],spverts[n].normal[2]) / (2. * M_PI) + 0.5;
		spverts[n].texCoord[1]= asin(spverts[n].normal[1]) / M_PI + .5;

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


	cbverts[0].texCoord[0] = 0;    cbverts[4].texCoord[0] = 1;
	cbverts[0].texCoord[1] = 0;    cbverts[4].texCoord[1] = 1;

	cbverts[1].texCoord[0] = 1;     cbverts[5].texCoord[0] = 1;
	cbverts[1].texCoord[1] = 0;    cbverts[5].texCoord[1] = 1;


	cbverts[2].texCoord[0] = 0;    cbverts[6].texCoord[0] = 1;
	cbverts[2].texCoord[1] = 1;     cbverts[6].texCoord[1] = 1;

	cbverts[3].texCoord[0] = 1;     cbverts[7].texCoord[0] = 1;
	cbverts[3].texCoord[1] = 1;     cbverts[7].texCoord[1] = 1;


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
	plyObjPos = new vec3[6]; // currently loads only 6 ply files at the same time

	plyObjPos[0] = vec3(0,-8,70);
	plyObjPos[1] = vec3(-50,2,0);
	plyObjPos[2] = vec3(50,-2,0);
	plyObjPos[3] = vec3(-50,-2,-35);
	plyObjPos[4] = vec3(-50,-2,35);
	plyObjPos[5] = vec3(50,-2,-35);
	plyObjPos[6] = vec3(50,-2,35);

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
					recip = -1 / sqrt (len);

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
					recip = 1 / sqrt (len);
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

			/*plyverts[p][i].texCoord[0] = 1;
			plyverts[p][i].texCoord[1] = 1;*/

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
	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);

	glDrawElements(GL_TRIANGLES,cubeindices,GL_UNSIGNED_INT,(char *) NULL+0);
}

void draw_cylinder()
{	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[1]);
	glEnableVertexAttribArray(c0); 
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
	glDrawElements(GL_TRIANGLE_STRIP,cylinderindices,GL_UNSIGNED_INT,(char *) NULL+0);
}

void draw_sphere()
{	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glEnableVertexAttribArray(c0); 
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
}

void draw_cone()
{	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[2]);
	glEnableVertexAttribArray(c0); 
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
	glDrawElements(GL_TRIANGLE_STRIP,coneindices,GL_UNSIGNED_INT,(char *) NULL+0);
}

void draw_ply(int i)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[i+4]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[i+4]);
	glEnableVertexAttribArray(c0); 
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
	glDrawElements(GL_TRIANGLES,facecount[i]*3,GL_UNSIGNED_INT,(char *) NULL+0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawModel()
{
	for(int i=0;i<totObj-4;i++)
	{
		modelStack.push(model);
		model = translate(model,vec3(plyObjPos[i])); //for others
		model=scale(model,vec3(300,300,280));  //for others

		//model = rotate(model,-90.0f,vec3(1,0,0)); // for monster only
		//model=scale(model,vec3(0.7,0.7,0.7)); // for monster only
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
	isCubeMap=0;
	glUniform1i(cubereq,isCubeMap);
	needTexture=1;
	glUniform1i(needtex,needTexture);	
	shininess=200;
	glUniform1i(m9,shininess);
	isBumpMap=0;
	isReflect=1;
	glUniform1i(reflectFlag,isReflect);
	glUniform1i(bumpreq,isBumpMap);
	glActiveTexture(GL_TEXTURE0+tid[0]);	
	glBindTexture(GL_TEXTURE_2D,tid[0]);
	glUniform1i(texLoc,tid[0]);	

	///////////////Draw base cube//////////////////	
	modelStack.push(model);	
	model=translate(model,vec3(0,-4,0));
	model=rotate(model,-90.0f,vec3(1,0,0));
	model=scale(model,vec3(600,600,2));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(1,0.87,0.67,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();	


	isReflect=0;
	glUniform1i(reflectFlag,isReflect);

	//glActiveTexture(GL_TEXTURE0+tid[1]);	
	//glBindTexture(GL_TEXTURE_2D,tid[1]);
	//glUniform1i(texLoc,tid[1]);	
	/////////////////Draw Back cube//////////////////////
	//modelStack.push(model);	
	//model=translate(model,vec3(0,295,-300));
	//model=scale(model,vec3(600,600,2));  
	//model=rotate(model,-180.0f,vec3(0,0,1));
	//normalMatrix=transpose(inverse(view*model));
	//diffuseTerm = vec4(1,0.87,0.67,1);
	//glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	//glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	//glUniform4fv(m5, 1,&diffuseTerm[0]);
	//draw_cube(); 
	//model=modelStack.top();

	//glActiveTexture(GL_TEXTURE0+tid[2]);	
	//glBindTexture(GL_TEXTURE_2D,tid[2]);
	//glUniform1i(texLoc,tid[2]);		
	/////////////////Draw left cube//////////////////////
	//modelStack.push(model);	
	//model=translate(model,vec3(-300,295,0));
	//model=rotate(model,-180.0f,vec3(1,0,0));
	//model=rotate(model,90.0f,vec3(0,1,0));
	//model=scale(model,vec3(600,600,2));  	
	//normalMatrix=transpose(inverse(view*model));
	//diffuseTerm = vec4(1,0.87,0.67,1);
	//glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	//glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	//glUniform4fv(m5, 1,&diffuseTerm[0]);
	//draw_cube(); 
	//model=modelStack.top();

	//glActiveTexture(GL_TEXTURE0+tid[3]);	
	//glBindTexture(GL_TEXTURE_2D,tid[3]);
	//glUniform1i(texLoc,tid[3]);	
	/////////////////Draw right cube//////////////////////
	//modelStack.push(model);	
	//model=translate(model,vec3(300,295,0));
	//model=rotate(model,180.0f,vec3(1,0,0));
	//model=rotate(model,-90.0f,vec3(0,1,0));
	//model=scale(model,vec3(600,600,2));  	
	//normalMatrix=transpose(inverse(view*model));
	//diffuseTerm = vec4(1,0.87,0.67,1);
	//glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	//glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	//glUniform4fv(m5, 1,&diffuseTerm[0]);
	//draw_cube(); 
	//model=modelStack.top();

	//glActiveTexture(GL_TEXTURE0+tid[4]);	
	//glBindTexture(GL_TEXTURE_2D,tid[4]);
	//glUniform1i(texLoc,tid[4]);		
	/////////////////Draw roof cube//////////////////////
	//modelStack.push(model);	
	//model=translate(model,vec3(0,595,0));
	//model=rotate(model,90.0f,vec3(1,0,0));
	//model=scale(model,vec3(600,600,2));    
	//normalMatrix=transpose(inverse(view*model));
	//diffuseTerm = vec4(1,0.87,0.67,1);
	//glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	//glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	//glUniform4fv(m5, 1,&diffuseTerm[0]);
	//draw_cube(); 
	//model=modelStack.top();	


	shininess=50;
	glUniform1i(m9,shininess);

	glActiveTexture(GL_TEXTURE0+tid[6]);	
	glBindTexture(GL_TEXTURE_2D,tid[6]);
	glUniform1i(texLoc,tid[6]);
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

	glActiveTexture(GL_TEXTURE0+tid[5]);	
	glBindTexture(GL_TEXTURE_2D,tid[5]);
	glUniform1i(texLoc,tid[5]);
	//Draw Roof Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,23,-45));
	model=rotate(model,-90.0f,vec3(1,0,0));
	model=scale(model,vec3(70,34,2));   	
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(1,1,1,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();	


	//Draw slant front roof Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,33,-37));
	model = rotate(model,-45.0f,vec3(1,0,0));
	model=scale(model,vec3(70,25,2));  
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(1,1,1,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();	

	//Draw slant back roof Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,33,-54));
	model = rotate(model,-135.0f,vec3(1,0,0));
	model=scale(model,vec3(70,25,2));  
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(1,1,1,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); // lower base of manson
	model=modelStack.top();

	glActiveTexture(GL_TEXTURE0+tid[7]);	
	glBindTexture(GL_TEXTURE_2D,tid[7]);
	glUniform1i(texLoc,tid[7]);
	//Draw base 1 of temple Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,-2,-45));
	model = rotate(model,-90.0f,vec3(1,0,0));
	model=scale(model,vec3(80,38,2));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(1,1,1,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();

	//Draw base 2  of temple Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,0,-45));
	model = rotate(model,-90.0f,vec3(1,0,0));
	model=scale(model,vec3(70,36,2));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(1,1,1,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();

	//Draw base 3 of temple Here /////////////////////////////////
	modelStack.push(model);	
	model=translate(model,vec3(-5,2,-45));
	model = rotate(model,-90.0f,vec3(1,0,0));
	model=scale(model,vec3(60,34,2));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(1,1,1,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();

	isBumpMap=1;
	glUniform1i(bumpreq,isBumpMap);
	isCubeMap=1; // use env map on the sphere to show that it works
	glUniform1i(cubereq,isCubeMap);
	needTexture=1;
	glUniform1i(needtex,needTexture);	
	glActiveTexture(GL_TEXTURE0+cubemap_texture);
	glActiveTexture(GL_TEXTURE0+tid[5]);	
	glBindTexture(GL_TEXTURE_2D,tid[5]);
	glUniform1i(texLoc,tid[5]);

	modelStack.push(model);	
	model=translate(model,vec3(-120,50,0));
	model = rotate(model,-90.0f,vec3(1,0,0));
	model=scale(model,vec3(25,25,25));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.2,0.4,0,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cylinder(); 
	model=modelStack.top();

	modelStack.push(model);	
	model=translate(model,vec3(80,60,-70));
	model=scale(model,vec3(25,25,25));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0.2,1,1,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_sphere(); 
	model=modelStack.top();

}

void drawStaticSceneTest()
{
	needTexture=1;
	glUniform1i(needtex,needTexture);
	isReflect=1;
	glUniform1i(reflectFlag,isReflect);
	isBumpMap=0;
	glUniform1i(bumpreq,isBumpMap);

	modelStack.push(model);		
	model=scale(model,vec3(25,25,25));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0,0,0,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_sphere(); 
	model=modelStack.top();

	isReflect=0;
	glUniform1i(reflectFlag,isReflect);

	modelStack.push(model);		
	model = translate(model,vec3(0,0,-100));
	model=scale(model,vec3(200,200,2));    
	normalMatrix=transpose(inverse(view*model));
	diffuseTerm = vec4(0,0.5,0,1);
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]);
	glUniform4fv(m5, 1,&diffuseTerm[0]);
	draw_cube(); 
	model=modelStack.top();
}

///////////////////////////////////////////////////////////////////////////////
void display() 
{

	//First Pass for depth rendering for Shadow Map
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle[0]); 
	glViewport(0,0,4096, 4096);   // the size of the viewport is the size of the texture 

	glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	if(shadeType==0)
	{
		programObject = perFragPhong;	
	}
	else if(shadeType==1)
	{
		programObject = cubeMap;
	}	
	else if(shadeType==2)
	{
		programObject = bumpMap;
	}
	else
	{
		programObject = shadowMap;
	}

	glUseProgram(programObject);	

	// get the location of the uniform variable in the shader
	c0 = glGetAttribLocation(programObject, "position"); 
	c1 = glGetAttribLocation(programObject, "normal"); 
	c2 = glGetAttribLocation(programObject, "color1");
	c3 = glGetAttribLocation(programObject, "texCoord");
	m1 = glGetUniformLocation(programObject, "projection"); 
	m2 = glGetUniformLocation(programObject, "model");
	m3 = glGetUniformLocation(programObject, "view");
	m4 = glGetUniformLocation(programObject, "normalMatrix");
	wnm = glGetUniformLocation(programObject, "worldNormalMatrix");
	m5 = glGetUniformLocation(programObject, "diffuseTerm");
	m6 = glGetUniformLocation(programObject, "kamb");
	m7 = glGetUniformLocation(programObject, "kdiff");
	m8 = glGetUniformLocation(programObject, "kspec");
	m9 = glGetUniformLocation(programObject, "shininess");
	texLoc = glGetUniformLocation(programObject, "Tex1");
	l1On = glGetUniformLocation(programObject, "L1");
	l2On = glGetUniformLocation(programObject, "L2");
	needtex = glGetUniformLocation(programObject, "needTexture");
	cmap =  glGetUniformLocation(programObject, "cubeMap");
	epos = glGetUniformLocation(programObject, "eyePos");
	cubereq = glGetUniformLocation(programObject, "isCubeMap");
	bumpreq = glGetUniformLocation(programObject, "isBumpMap");
	normalTex = glGetUniformLocation(programObject, "normal_texture");	
	shadowTex = glGetUniformLocation(programObject, "ShadowMap");
	reflectTex = glGetUniformLocation(programObject, "reflectionMap");
	lightView = glGetUniformLocation(programObject, "lightViewMatrix");
	reflectFlag = glGetUniformLocation(programObject, "isReflect");
	m10 = glGetUniformLocation(programObject, "invMVPMat");

	if(polygonMode)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // comment this line to enable polygon shades
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); 	

	model = mat4(1.0f);	//Important step..
	mat4 projection = perspective(70.0f,1.0f,25.0f,1500.0f); 
	view = lookAt(vec3(-20,200,300),COI,vec3(0.0,1.0, 0.0)); 

	//Form lights view matrix
	lightViewMatrix = projection*view;

	model =rotate(model, x_angle, vec3(0.0f, 1.0f, 0.0f));   
	model = rotate(model, y_angle, vec3(1.0f, 0.0f, 0.0f));   
	model = scale(model, vec3(scale_size, scale_size, scale_size));  

	normalMatrix=transpose(inverse(view*model));// normalMatrix is inverse transpose of modelView matrix
	worldNormalMatrix=transpose(inverse(model)); // normal matrix in world space.. required for cube Env mapping
	invMVP = inverse(projection*view); //inverse proj matrix 

	// construct the modelview and modelview projection matrices 	 
	glUniformMatrix4fv(m1, 1, GL_FALSE, &projection[0][0]); // pass the viewProjection matrix to the shader as a uniform 
	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]); // pass the model matrix to the shader as a uniform 
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);  // pass view matrix as uniform to shader
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]); // pass normal matrix (inverse transpose of modelView matrix as uniform to shader
	glUniformMatrix4fv(wnm, 1, GL_FALSE, &worldNormalMatrix[0][0]);
	glUniformMatrix4fv(lightView, 1, GL_FALSE, &lightViewMatrix[0][0]);
	glUniformMatrix4fv(m10, 1, GL_FALSE, &invMVP[0][0]);
	glUniform4fv(m5,1,&diffuseTerm[0]); 
	glUniform1f(m6,kamb);
	glUniform1f(m7,kdiff);
	glUniform1f(m8,kspec);
	glUniform1i(m9,shininess);
	glUniform1i(l1On,light1);
	glUniform1i(l2On,light2);	
	glUniform1i(needtex,needTexture);
	glUniform3fv(epos,1,&eyePos[0]);
	glUniform1i(cubereq,isCubeMap);
	glUniform1i(bumpreq,isBumpMap);
	glUniform1i(reflectFlag,isReflect);
	glUniform1i(texLoc,1);	
	glUniform1i(cmap,0);
	glUniform1i(normalTex,tid[8]);	
	glUniform1i(shadowTex,depthTex);
	glUniform1i(reflectTex,reflectionTex);

	glActiveTexture(GL_TEXTURE0+tid[8]);	
	glBindTexture(GL_TEXTURE_2D,tid[8]);
	glUniform1i(normalTex,tid[8]);

	//******************************************************//
	/********Do all drawing stuffs here*********************/

	if(shadeType!=1)
	{
		needTexture=0;
		glUniform1i(needtex,needTexture);
		isCubeMap=0;	
		glUniform1i(cubereq,isCubeMap);	
		drawModel(); // draws ply objects	
	}
	else if(shadeType==1)
	{
		needTexture=1;
		glUniform1i(needtex,needTexture);
		isCubeMap=1;	
		glUniform1i(cubereq,isCubeMap);	
		drawModel(); // draws ply objects	
	}

	//Create Static Scene
	drawStaticScene();	// static scene is always drawn using phong shading	

	//drawStaticSceneTest();

	needTexture=0;
	glUniform1i(needtex,needTexture);
	isBumpMap=1;
	glUniform1i(bumpreq,isBumpMap);

	//Dynamic Object transformations start here for moving the blobby man

	modelStack.push(model);
	Robo.draw(60,0,30); //( Initial position of the robot (0,0,30) this can be changed   
	modelStack.pop();

	/*********************************************************************************************/
	/*********************************************************************************************/
	/*   Second Pass of rendering  For Reflection map*/
	/*********************************************************************************************/
	/*********************************************************************************************/

	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle[1]); 
	glViewport(0,0,800,800);   // the size of the viewport is the size of the texture 

	glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	model = mat4(1.0f);		
	projection = perspective(70.0f,1.0f,15.0f,1500.0f);
	view = lookAt(eyePos,COI,vec3(0.0,1.0,0.0));		
	view = scale(view,vec3(1.0,-1.0,1.0));
	invMVP = inverse(projection*view); //inverse proj matrix 
	model =rotate(model, x_angle, vec3(0.0f, 1.0f, 0.0f));   
	model = rotate(model, -y_angle, vec3(1.0f, 0.0f, 0.0f));   // angle is negative here because we have flipped y values.
	model = scale(model, vec3(scale_size, scale_size, scale_size)); 
	normalMatrix=transpose(inverse(view*model));// normalMatrix is inverse transpose of modelView matrix
	worldNormalMatrix=transpose(inverse(model)); // normal matrix in world space.. required for cube Env mapping

	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]); // pass the model matrix to the shader as a uniform
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(m1, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]); 
	glUniformMatrix4fv(wnm, 1, GL_FALSE, &worldNormalMatrix[0][0]);
	glUniformMatrix4fv(m10, 1, GL_FALSE, &invMVP[0][0]);

	if(shadeType!=1)
	{
		needTexture=0;
		glUniform1i(needtex,needTexture);
		isCubeMap=0;	
		glUniform1i(cubereq,isCubeMap);	
		drawModel(); // draws ply objects	
	}
	else if(shadeType==1)
	{
		needTexture=1;
		glUniform1i(needtex,needTexture);
		isCubeMap=1;	
		glUniform1i(cubereq,isCubeMap);	
		drawModel(); // draws ply objects	
	}	

	//Create Static Scene
	drawStaticScene();	// static scene is always drawn using phong shading
	//drawStaticSceneTest();

	needTexture=0;
	glUniform1i(needtex,needTexture);
	isBumpMap=1;
	glUniform1i(bumpreq,isBumpMap);

	//Dynamic Object transformations start here for moving the blobby man
	modelStack.push(model);
	Robo.draw(60,0,30); //( Initial position of the robot (0,0,30) this can be changed   
	modelStack.pop();

	/*********************************************************************************************/
	/*********************************************************************************************/
	/*   Render Pass of Scene  */
	/*********************************************************************************************/
	/*********************************************************************************************/
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	glViewport(0,0,800,800); // the size of the viewport is the size of the texture 

	needTexture=0;
	glUniform1i(needtex,needTexture);
	isCubeMap=0;	
	glUniform1i(cubereq,isCubeMap);	
	isBumpMap=0;
	glUniform1i(bumpreq,isBumpMap);	

	glEnable(GL_DEPTH_TEST);  // need depth test to correctly draw 3D objects 
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	model = mat4(1.0f);		
	invMVP = inverse(projection*view); //inverse proj matrix 
	projection = perspective(70.0f,1.0f,15.0f,1500.0f);
	view = lookAt(eyePos,COI,vec3(0.0,1.0, 0.0));
	model =rotate(model, x_angle, vec3(0.0f, 1.0f, 0.0f));   
	model = rotate(model, y_angle, vec3(1.0f, 0.0f, 0.0f));   
	model = scale(model, vec3(scale_size, scale_size, scale_size));  
	normalMatrix=transpose(inverse(view*model));// normalMatrix is inverse transpose of modelView matrix
	worldNormalMatrix=transpose(inverse(model)); // normal matrix in world space.. required for cube Env mapping


	glUniformMatrix4fv(m2, 1, GL_FALSE, &model[0][0]); // pass the model matrix to the shader as a uniform
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(m1, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m4, 1, GL_FALSE, &normalMatrix[0][0]); 
	glUniformMatrix4fv(wnm, 1, GL_FALSE, &worldNormalMatrix[0][0]);
	glUniformMatrix4fv(m10, 1, GL_FALSE, &invMVP[0][0]);

	if(shadeType!=1)
	{
		needTexture=0;
		glUniform1i(needtex,needTexture);
		isCubeMap=0;	
		glUniform1i(cubereq,isCubeMap);	
		drawModel(); // draws ply objects	
	}
	else if(shadeType==1)
	{
		needTexture=1;
		glUniform1i(needtex,needTexture);
		isCubeMap=1;	
		glUniform1i(cubereq,isCubeMap);	
		drawModel(); // draws ply objects	
	}	


	glActiveTexture(GL_TEXTURE0+depthTex);	
	glBindTexture(GL_TEXTURE_2D,depthTex);
	glUniform1i(shadowTex,depthTex);

	glActiveTexture(GL_TEXTURE0+reflectionTex);	
	glBindTexture(GL_TEXTURE_2D,reflectionTex);
	glUniform1i(reflectTex,reflectionTex);

	//Create Static Scene
	drawStaticScene();	// static scene is always drawn using phong shading

	//drawStaticSceneTest();

	needTexture=1;
	glUniform1i(needtex,needTexture);
	isBumpMap=1;
	glUniform1i(bumpreq,isBumpMap);

	//Dynamic Object transformations start here for moving the blobby man
	modelStack.push(model);
	Robo.draw(60,0,30); //( Initial position of the robot (0,0,30) this can be changed   
	modelStack.pop();

	glutSwapBuffers(); 
	glUseProgram(0);
}

////////////    Main FUnction   //////////////////////////////////////////

int main(int argc, char** argv) 
{

	if(argc==1)
	{
		cout<<"No data file is loaded"<<endl;
		system("pause");
		exit(0);
	}

	totObj=(argc-1)+4;
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
		fclose(fp1);
	}

	//reads the input texture ppm image and creats texture data

	base = read_Image("negative_y.ppm");
	back = read_Image("negative_z.ppm");
	leftIm = read_Image("negative_x.ppm");
	rightIm = read_Image("positive_x.ppm");
	roof = read_Image("positive_y.ppm");
	marbleTexture = read_Image("marbleTexture.ppm");	
	tiles = read_Image("tiles.ppm");
	piller = read_Image("piller.ppm");	
	bumpNormal = read_Image("bumpNormal.ppm");


	cubeMapImages[0] = read_Image("positive_x.ppm");
	cubeMapImages[1] = read_Image("negative_x.ppm");
	cubeMapImages[2] = read_Image("positive_y.ppm");
	cubeMapImages[3] = read_Image("negative_y.ppm");
	cubeMapImages[4] = read_Image("positive_z.ppm");
	cubeMapImages[5] = read_Image("negative_z.ppm");

	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
	glutInitWindowSize(width,height); 

	glutCreateWindow("GLSL Texture and Cube Environment Map"); 
	glutDisplayFunc(display); 
	glutMouseFunc(mymouse); 
	glutMotionFunc(mymotion);
	glutKeyboardFunc(mykey); 
	glutSpecialFunc(SpecialKey);	

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
	cubeMap = SetupGLSL("cubeMap"); // create cubemap shader
	bumpMap = SetupGLSL("bumpMap"); // create bump Map shader
	shadowMap = SetupGLSL("shadowMap"); // create bump Map shader

	InitCylinder(nslices, nstacks,1,1,1); 
	InitCone(nslices,nstacks,1,1); 	
	InitSphere(nslices,nstacks,1);
	InitCube(1);
	InitPly(argv);

	InitVBO(nslices, nstacks); 
	InitFBO(); 
	Robo.angle=0;

	glutMainLoop(); 
} 