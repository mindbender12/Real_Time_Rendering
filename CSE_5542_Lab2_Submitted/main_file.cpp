///////////////////////////////////////////////////////////////////////
//          Lab2 of CSE 5542 Course                                  //
//          Implemented by: Soumya Dutaa                             //
//          Graduate Student                                         //
//          duttas@cse.ohio-state.edu                                //
///////////////////////////////////////////////////////////////////////

// Compile: g++ main_file.cpp -o out -w -I/home/sdutta/glm -lGL -lGLU -lGLEW -lglut
// Run: ./out
// *********************************************************************************8

#include <iostream>
#include <cmath>
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <glm/glm.hpp>  
#include <ctime>
#include <vector>

using namespace glm;
using namespace std;

const float PI=3.14f;

typedef struct 
{
	float location[4]; 
	float normal[4]; 	
} Vertex; 


int cylinderindices,cubeindices,coneindices,sphereindices; 
Vertex *cyverts;   // cylinder vertices
Vertex *spverts;   // sphere vertices
Vertex *cnverts;   // cone vertices
Vertex *cbverts;   //cube vertices
GLuint *cyindices,*cnindices,*cbindices,*spindices;
GLuint vboHandle[4];   //VBO that contains interleaved positions and colors 
GLuint indexVBO[4]; 
float rotAngle=0; 
float rotCone=0;
float walkLen=0;
int walk=1;
int rotAngle1=0;
int rotateAll=0;
float turn=0;
int rotLArm=0;
int rotRArm=0;
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

void builRobot();
void torso();
void draw_lower_body();
void draw_left_thigh();
void draw_right_thigh();
void draw_left_calf();
void draw_right_calf();
void draw_upper_body();
void left_upper_arm();
void left_lower_arm();
void right_upper_arm();
void right_lower_arm();
void neck();
void head();
void left_eye();
void right_eye();
void nose();
void lip();
void left_hand();
void right_hand();
void left_toe();
void right_toe();

void builRobot()
{	
	//fixed parts
	torso();
	draw_lower_body();
	draw_upper_body();
	neck();

	glPushMatrix();
	glRotatef(rotHeadAngle,0,1,0); // rotate the head and look around
	head();
	left_eye();
	right_eye();
	nose();
	lip();
	glPopMatrix();
	//Fixed body parts end here ...................//

	/*******************************************************************/
	/***********************Arm Section Begin***************************/
	glPushMatrix();
	if(walk==0 && movement)
	{
		glRotatef(rotAngle,1,0,0);
		glTranslatef(0,0,-walkLen-4);
	}	

	else if(walk==1 && movement)
	{
		glRotatef(-rotAngle,1,0,0);
		glTranslatef(0,0,walkLen);
	}

	if(moveBack==0 && !movement)
	{
		glRotatef(rotAngle,1,0,0);
		glTranslatef(0,0,-walkLen-4);
	}	

	else if(moveBack==1 && !movement)
	{
		glRotatef(-rotAngle,1,0,0);
		glTranslatef(0,0,walkLen);
	}

	if(rotL==1)
	{
		glRotatef(-rotLArm,1,0,0);
		glTranslatef(0,-5.8,13);
	}

	left_upper_arm();
	left_lower_arm();
	left_hand();
	glPopMatrix();

	glPushMatrix();  // lower and upper right arm group starts	

	if(walk==0 && movement)
	{
		glRotatef(-rotAngle,1,0,0);
		glTranslatef(0,0,walkLen+4);
	}	

	else if(walk==1 && movement)
	{
		glRotatef(rotAngle,1,0,0);
		glTranslatef(0,0,-walkLen);
	}

	if(moveBack==0 && !movement)
	{
		glRotatef(-rotAngle,1,0,0);
		glTranslatef(0,0,walkLen+4);
	}	

	else if(moveBack==1 && !movement)
	{
		glRotatef(rotAngle,1,0,0);
		glTranslatef(0,0,-walkLen);
	}

	if(rotR==1)
	{
		glRotatef(-rotRArm,1,0,0);
		glTranslatef(0,-5.8,13);
	}

	right_upper_arm();	
	right_lower_arm();
	right_hand();

	glPopMatrix();   // lower and upper right arm group ends

	/*******************************************************************/
	/***********************Arm Section End***************************/

	/*******************************************************************/
	/***********************Leg Section Begin***************************/

	glPushMatrix(); // lower and upper left leg group starts

	if(walk==0 && movement)
	{
		glRotatef(-rotAngle,1,0,0);
		glTranslatef(0,0,walkLen);
	}	

	else if(walk==1 && movement)
	{
		glRotatef(rotAngle,1,0,0);
		glTranslatef(0,0,-walkLen);
	}

	if(moveBack==0 && !movement)
	{
		glRotatef(-rotAngle,1,0,0);
		glTranslatef(0,0,walkLen);
	}	

	else if(moveBack==1 && !movement)
	{
		glRotatef(rotAngle,1,0,0);
		glTranslatef(0,0,-walkLen);
	}

	draw_left_thigh();	
	draw_left_calf();
	left_toe();

	glPopMatrix();	// lower and upper left leg group ends

	glPushMatrix(); // lower and upper right leg group starts


	if(walk==0 && movement)
	{
		glRotatef(rotAngle,1,0,0);
		glTranslatef(0,0,-walkLen-1);
	}
	else if(walk==1 && movement)
	{
		glRotatef(-rotAngle1,1,0,0);
		glTranslatef(0,0,walkLen);

	}

	if(moveBack==0 && !movement)
	{
		glRotatef(rotAngle,1,0,0);
		glTranslatef(0,0,-walkLen-1);
	}
	else if(moveBack==1 && !movement)
	{
		glRotatef(-rotAngle1,1,0,0);
		glTranslatef(0,0,walkLen);
	}

	draw_right_thigh();	
	draw_right_calf();
	right_toe();

	glPopMatrix(); // lower and upper right leg group ends

	/*******************************************************************/
	/***********************Leg Section End****************************/
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

	void draw(float startx,float starty,float startz)
	{
		glTranslatef(pos[0],pos[1],pos[2]);
		glTranslatef(startx,starty,startz);
		glRotatef(angle, 0,1,0);               
		builRobot(); // pass the starting location of the robot
	}
};

//Creating a robot struct
Robot Robo;

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
	if (xform_mode==XFORM_ROTATE) 
	{
		x_angle += (x - press_x)/5.0; 
		if (x_angle > 180) x_angle -= 360; 
		else if (x_angle <-180) x_angle += 360; 
		press_x = x; 

		y_angle += (y - press_y)/5.0; 
		if (y_angle > 180) y_angle -= 360; 
		else if (y_angle <-180) y_angle += 360; 
		press_y = y; 
	}
	else if (xform_mode == XFORM_SCALE)
	{
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
		pos=pos+1;	
		rotHeadAngle=0;

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
			rotAngle=30;
			walkLen=3;

		}
		else if(walk==1)
		{
			rotAngle=15;
			rotAngle1=30;
			walkLen=3;				
		}

		Robo.moveForward(walkLen);
	}

	if(key=='b')
	{
		pos=pos+1;
		rotHeadAngle=0;

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
			rotAngle=30;
			walkLen=3;
		}

		else if(moveBack==1)
		{
			rotAngle=15;
			rotAngle1=30;
			walkLen=3;		
		}

		Robo.moveBackward(walkLen);
	}


	if(key=='t')
	{
		turn=15;
		Robo.turnRight(turn);
	}

	if(key=='T')
	{		
		turn=15;
		Robo.turnLeft(turn);	
	}

	if(key=='l')
	{
		if(rotL==0)
			rotL=1;
		else
			rotL=0;

		rotLArm=60;		
		walk=-1;
		moveBack=-1;

	}

	if(key=='r')
	{
		if(rotR==0)
			rotR=1;
		else
			rotR=0;	

		rotRArm=60;	
		walk=-1;
		moveBack=-1;
	}	

	if(key=='h')
	{
		if(rotHeadDir==0)
			rotHeadDir=1;
		else
			rotHeadDir=0;		
		if(rotHeadAngle<90)
		rotHeadAngle=rotHeadAngle+30;		
	}

	if(key=='H')
	{
		if(rotHeadDir==0)
			rotHeadDir=1;
		else
			rotHeadDir=0;

		if(rotHeadAngle>-90)
		rotHeadAngle=rotHeadAngle-30;	
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

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);   // bind the first handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, spverts, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, spindices, GL_STATIC_DRAW);  // load the index data 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up

	//Initialize Cylinder VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);  // bind the second handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, cyverts, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexVBO[1]); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, cyindices, GL_STATIC_DRAW);  // load the index data 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up

	//Initialize Cone VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]);  // bind the second handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, cnverts, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexVBO[2]); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, cnindices, GL_STATIC_DRAW);  // load the index data 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up	


	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[3]);   // bind the first handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*cubenvertices, cbverts, GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*cubenindices, cbindices, GL_STATIC_DRAW);  // load the index data 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up

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
			cyverts[idx].normal[3] = 1.0;
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
	cbverts[0].location[3] =1;        cbverts[4].location[3] =1;

	cbverts[1].location[0] = (float)size/2;     cbverts[5].location[0] = (float)size/2;
	cbverts[1].location[1] = -(float)size/2;    cbverts[5].location[1] = (float)size/2;
	cbverts[1].location[2] = (float)size/2;     cbverts[5].location[2] = -(float)size/2;
	cbverts[1].location[3] =1;        cbverts[5].location[3] =1;

	cbverts[2].location[0] = -(float)size/2;    cbverts[6].location[0] = -(float)size/2;
	cbverts[2].location[1] = (float)size/2;     cbverts[6].location[1] = -(float)size/2;
	cbverts[2].location[2] = (float)size/2;     cbverts[6].location[2] = -(float)size/2;
	cbverts[2].location[3] =1;        cbverts[6].location[3] =1;

	cbverts[3].location[0] = (float)size/2;     cbverts[7].location[0] = (float)size/2;
	cbverts[3].location[1] = (float)size/2;     cbverts[7].location[1] = -(float)size/2;
	cbverts[3].location[2] = (float)size/2;     cbverts[7].location[2] = -(float)size/2;
	cbverts[3].location[3] =1;        cbverts[7].location[3] =1;


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

}

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
void draw_lower_body()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0.5,0,0,0);
	glTranslatef(0,7.5,0);
	glScalef(3.5,1,1);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void right_hand()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0,0,0,0);
	glTranslatef(3.2,7.3,0);
	glScalef(0.7,1.2,0.3);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void left_hand()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0,0,0,0);
	glTranslatef(-3.2,7.3,0);
	glScalef(0.7,1.2,0.3);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void right_toe()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0,0,0,0);
	glTranslatef(2.8,-1.4,1.1);
	glScalef(0.8,0.5,1.5);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void left_toe()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0,0,0,0);
	glTranslatef(-2.8,-1.4,1.1);
	glScalef(0.8,0.5,1.5);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}


void draw_left_thigh()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16);
	glColor4f(0,0,0,0);
	glTranslatef(-2.8,5.3,0);
	glScalef(1,2,1);  
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void draw_right_thigh()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 
	glColor4f(0,0,0,0);
	glTranslatef(2.8,5.3,0);  
	glScalef(1,2,1);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}


void draw_left_calf()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 
	glColor4f(1,1,1,0);
	glTranslatef(-2.8,1.5,0);
	glScalef(1,3.5,1); 
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void draw_right_calf()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(1,1,1,0);
	glTranslatef(2.8,1.5,0); 
	glScalef(1,3.5,1); 
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}


void torso()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16);
	glColor4f(0.5,0,0,0);
	glTranslatef(0,11,0);
	glScalef(2.2,3.5,2);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void draw_upper_body()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0.5,0,0,0);
	glTranslatef(0,14.5,0);
	glScalef(4,1,1);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}


void left_upper_arm()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0,0,0,0);
	glTranslatef(-3.2,13,0); 
	glScalef(1,2,1);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void left_lower_arm()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(1,1,1,0);
	glTranslatef(-3.2,9.5,0);   
	glScalef(1,2,1);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void right_upper_arm()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0,0,0,0);
	glTranslatef(3.2,13,0); 
	glScalef(1,2,1);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void right_lower_arm()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(1,1,1,0);
	glTranslatef(3.2,9.5,0); 
	glScalef(1,2,1);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void neck()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0.5,0.5,0.5,0);
	glTranslatef(0,15,0);
	glScalef(0.8,2.4,0.6);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void head()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(0.4,0,0,0);
	glTranslatef(0,18,0);
	glScalef(1.8,2.2,1);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void left_eye()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 
	glColor4f(1,1,1,0);
	glTranslatef(-0.5,19,1);
	glScalef(0.3,0.3,0.3);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}


void right_eye()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(1,1,1,0);
	glTranslatef(0.5,19,1);
	glScalef(0.3,0.3,0.3);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

void nose()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(1,1,1,0);
	glTranslatef(0,18,1);
	glScalef(0.2,0.5,0.2);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}


void lip()
{
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 	
	glColor4f(1,1,1,0);
	glTranslatef(0,17,1);
	glScalef(0.7,0.2,0.2);
	glDrawElements(GL_TRIANGLE_STRIP,sphereindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawGlobalCoordinateFranme()
{
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,10);
	glColor3f(0,1,0);
	glVertex3f(0,0,0);
	glVertex3f(0,10,0);	
	glColor3f(1,0,0);
	glVertex3f(0,0,0);
	glVertex3f(10,0,0);
	glEnd();
	glLineWidth(1);	

}

void drawStaticScene()
{
	glPushMatrix(); // For static scene

	//*****************CUBE SECTION***************************************************************************//
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[3]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[3]);
	glEnableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
	glEnableClientState(GL_NORMAL_ARRAY); // enable the normal array on the client side
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 

	glPushMatrix(); //for base cube
	glColor3f(0.5,0.5,0.4);
	glScalef(200,1,200);
	glTranslatef(0,-3.8,0);
	glDrawElements(GL_TRIANGLES,cubeindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix(); //for base cube

	glPushMatrix(); //body of the house
	glColor3f(0.5,0.9,0.5);
	glScalef(25,15,9);
	glTranslatef(0,0.3,-6);
	glDrawElements(GL_TRIANGLES,cubeindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();

	glPushMatrix(); //door of the house
	glColor3f(0,0,0);
	glScalef(3.5,9,1.2);
	glTranslatef(0,0.2,-40);
	glDrawElements(GL_TRIANGLES,cubeindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();


	//*****************CUBE SECTION ENDS***************************************************************************//

	//*****************CYLINDER SECTION ***************************************************************************//

	glPushMatrix(); //For Cylinder
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[1]); 
	glEnableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
	glEnableClientState(GL_NORMAL_ARRAY); // enable the normal array on the client side
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 

	glColor3f(1,0.3,0);

	glPushMatrix();
	glScalef(1,8,1);
	glRotatef(90,1,0,0);
	glTranslatef(20,-20,-0.5);
	glDrawElements(GL_TRIANGLE_STRIP,cylinderindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();//For Cylinder

	glPushMatrix();
	glScalef(1,8,1);
	glRotatef(90,1,0,0);
	glTranslatef(-20,-20,-0.5);
	glDrawElements(GL_TRIANGLE_STRIP,cylinderindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();//For Cylinder

	//*****************CYLINDER SECTION ENDS***************************************************************************//

	//*****************CONE SECTION***************************************************************************//
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[2]); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[2]); 
	glEnableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
	glEnableClientState(GL_NORMAL_ARRAY); // enable the normal array on the client side
	glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 


	glPushMatrix(); //For Cone
	glColor3f(0.18,0.54,0.32);
	glScalef(4,8,4);
	glRotatef(-90,1,0,0);
	glTranslatef(5,5,0.5);
	glDrawElements(GL_TRIANGLE_STRIP,coneindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();//For cone

	glPushMatrix(); //For Cone
	glColor3f(0.5,0.8,0.19);
	glScalef(4,8,4);
	glRotatef(-90,1,0,0);
	glTranslatef(5,5,0.9);
	glDrawElements(GL_TRIANGLE_STRIP,coneindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();//For cone

	glPushMatrix(); //For Cone
	glColor3f(0,1,0);
	glScalef(4,8,4);
	glRotatef(-90,1,0,0);
	glTranslatef(5,5,1.2);
	glDrawElements(GL_TRIANGLE_STRIP,coneindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();//For cone

	glPushMatrix(); //For Cone
	glColor3f(0.18,0.54,0.32);
	glScalef(4,8,4);
	glRotatef(-90,1,0,0);
	glTranslatef(-5,5,0.5);
	glDrawElements(GL_TRIANGLE_STRIP,coneindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();//For cone

	glPushMatrix(); //For Cone
	glColor3f(0.5,0.8,0.19);
	glScalef(4,8,4);
	glRotatef(-90,1,0,0);
	glTranslatef(-5,5,0.9);
	glDrawElements(GL_TRIANGLE_STRIP,coneindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();//For cone

	glPushMatrix(); //For Cone
	glColor3f(0,1,0);
	glScalef(4,8,4);
	glRotatef(-90,1,0,0);
	glTranslatef(-5,5,1.2);
	glDrawElements(GL_TRIANGLE_STRIP,coneindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();//For cone


	glPushMatrix(); //For roof of house
	glColor3f(1,0.1,0.1);
	glScalef(17,10,8);
	glRotatef(-90,1,0,0);
	glTranslatef(0,6.7,1.2);
	glDrawElements(GL_TRIANGLE_STRIP,coneindices,GL_UNSIGNED_INT,(char *) NULL+0);
	glPopMatrix();//For cone

	//*****************CONE SECTION ENDS***************************************************************************//

	glPopMatrix(); //For static scene ends	

}

///////////////////////////////////////////////////////////////////////////////
void display() 
{ 
	glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 
	glClearColor(0.5,0.82,0.9,1); 
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	glShadeModel(GL_SMOOTH);

	//All color and material stuffs go here
	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0); 
	glEnable(GL_NORMALIZE);   // normalize normals 

	glEnable(GL_COLOR_MATERIAL); 
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 

	// set up the parameters for lighting 
	GLfloat light_ambient[] = {0,0,0,1}; 
	GLfloat light_diffuse[] = {.8,.6,.6,1};
	GLfloat light_specular[] = {1,1,1,1}; 
	GLfloat light_pos[] = {20,40,50,1}; 

	glLightfv(GL_LIGHT0,GL_AMBIENT, light_ambient); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); 


	GLfloat mat_specular[] = {.9, .9, .9,1}; 
	GLfloat mat_shine[] = {10}; 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shine); 
	//color specs ends ////////////////////////////////////////

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // comment this line to enable polygon shades
	
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(75, float(width)/(float)height, 0.001, 1000); // resizing screen will maintain aspect ratio

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 

	
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos); 

	//gluLookAt(-18,40,60,0,0,0,0,1,0); //Camera set up 	

	gluLookAt(0,20,60,0,0,0,0,1,0); //Camera set up 

	//These are for rotating and zooming effects
	glRotatef(x_angle, 0, 1,0); 
	glRotatef(y_angle, 1,0,0); 
	glScalef(scale_size, scale_size, scale_size); 

	//drawGlobalCoordinateFranme(); //Just for getting an idea co-ordinate frame

	//******************************************************//
	/********Do all drawing stuffs here*********************/

	//Create Static Scene
	drawStaticScene();

	//Dynamic Object transformations start here for moving the blobby man	

	Robo.draw(0,0,0); //( Initial position of the robot (0,0,0) this can be changed

	glDisableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
	glDisableClientState(GL_NORMAL_ARRAY); // enable the normal array on the client side
	glutSwapBuffers(); 
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
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
	glutInitWindowSize(width,height); 

	glutCreateWindow("Blobby Man Walking ;-)"); 
	glutDisplayFunc(display); 
	glutMouseFunc(mymouse); 
	glutMotionFunc(mymotion);
	glutKeyboardFunc(mykey); 
	glutReshapeFunc(reshape);

	// initialize GLEW 
	GLenum err = glewInit(); 

	if ( err != GLEW_OK) 
		cout<<" Error initializing GLEW! \n"<<endl; 
	else cout<<"Initializing GLEW succeeded!\n"<<endl; 

	// define the discretion level for the cylinder 
	int nslices, nstacks; 
	nslices = 25;  
	nstacks = nslices/2+1; 

	InitCylinder(nslices, nstacks,1,1,1); 
	InitCone(nslices,nstacks,1,1); 	
	InitSphere(nslices,nstacks,1);
	InitCube(1);

	InitVBO(nslices, nstacks); 
	Robo.angle=0;

	glutMainLoop(); 
} 