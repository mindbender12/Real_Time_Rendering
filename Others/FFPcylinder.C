
////////////////////////////////////////////////////////////////////////////////////////////////
// 
// The purpose of this program is to use vertex buffer objects (VBOs) to define a simple 3D cylinder 
// Note that we will not use glBegin/End any more since they have been deprecated in OpenGL 3.x 
// 
// A very simple shader (cube_simple.{vert,frag}) is uesd in this example
//
// In this example, the triangles vertices and colors are listed in separate arrays and VBOs. We also 
// use a triangle index array (GL_ELEMENT_ARRAY_BUFFER) to assemble triangles 
// vertices. As a result, we use glDrawArrays() to draw. 
//
// Simple 3D viewing is set up, using the OpenGL mathematics (glm) library. This is to replace the 
// OpenGL 2.x methods (GL_MODELVIEW, GL_PROJECTIon, etc) that have been deprecated. 
//
// Han-Wei Shen, July 2012 @ Ohio State University 
// hwshen@cse.ohio-state.edu 
// 
////////////////////////////////////////////////////////////////////////////////////////////////

#include <GL/glew.h> 
#include <GLUT/glut.h> 
#include <OpenGL/gl.h> 

#include <stdio.h>
#include <stdlib.h>

#include<math.h>

typedef struct 
{
  float location[4]; 
  float normal[4]; 
  float color[4]; 
} Vertex; 

int nindices; 
Vertex *cyverts;   // cylinder vertices
GLuint *cindices; 

GLuint vboHandle[1];   // a VBO that contains interleaved positions and colors 
GLuint indexVBO; 

/////////////////////////////////
// glut mouse control 
// 
int xform_mode = 0; 
#define XFORM_NONE    0 
#define XFORM_ROTATE  1
#define XFORM_SCALE 2 

int press_x, press_y; 
int release_x, release_y; 
float x_angle = 0.0; 
float y_angle = 0.0; 
float scale_size = 1; 

////////////////////////////////////////////////////////////////////////////////////

void InitCylinder(int nslices, int nstacks, float r, float g, float b) 
{
  int nvertices = nslices * nstacks; 
  cyverts = new Vertex[nvertices]; 

  printf(" M PI = %f\n", M_PI); 
  float Dangle = 2*M_PI/(float)(nslices-1); 

  for (int j =0; j<nstacks; j++)
    for (int i=0; i<nslices; i++) {
      int idx = j*nslices + i; // mesh[j][i] 
      float angle = Dangle * i; 
      cyverts[idx].location[0] = cyverts[idx].normal[0] = cos(angle); 
      cyverts[idx].location[1] = cyverts[idx].normal[1] = sin(angle); 
      cyverts[idx].location[2] = j*1.0/(float)(nstacks-1); 
      cyverts[idx].normal[2] = 0.0; 
      cyverts[idx].location[3] = 1.0;  cyverts[idx].normal[3] = 0.0; 
      cyverts[idx].color[0] = r; cyverts[idx].color[1] = g; cyverts[idx].color[2] = b; 
      cyverts[idx].color[3] = 1.0; 
    }
  // now create the index array 

  nindices = (nstacks-1)*2*(nslices+1); 
  cindices = new GLuint[nindices]; 
  int n = 0; 
  for (int j =0; j<nstacks-1; j++)
    for (int i=0; i<=nslices; i++) {
      int mi = i % nslices;  
      int idx = j*nslices + mi; // mesh[j][mi] 
      int idx2 = (j+1) * nslices + mi; 
      cindices[n++] = idx; 
      cindices[n++] = idx2; 
    }
}

////////////////////////////////////////////////////////////////////////////////////

void display() 
{ 
  glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 
  glClearColor(0,0,0,1); 
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

  glEnable(GL_LIGHTING); 
  glEnable(GL_LIGHT0); 
  glEnable(GL_NORMALIZE);   // normalize normals 

  glEnable(GL_COLOR_MATERIAL); 
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 

  // set up the parameters for lighting 
  GLfloat light_ambient[] = {0,0,0,1}; 
  GLfloat light_diffuse[] = {.6,.6,.6,1};
  GLfloat light_specular[] = {1,1,1,1}; 
  GLfloat light_pos[] = {10,10,10,1}; 

  glLightfv(GL_LIGHT0,GL_AMBIENT, light_ambient); 
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); 
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); 


  GLfloat mat_specular[] = {.9, .9, .9,1}; 
  GLfloat mat_shine[] = {10}; 
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular); 
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shine); 

  //  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); 

  glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO); 

  glEnableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
  glEnableClientState(GL_NORMAL_ARRAY); 
  glEnableClientState(GL_COLOR_ARRAY); // enable the color array on the client side

  // Tells OpenGL how to walk through the VBOs, i.e., how the data are packed 
  // number of coordinates per vertex (4 here), type of the coordinates, 
  // stride between consecutive vertices, and pointers to the first coordinate

  glVertexPointer(4,GL_FLOAT, sizeof(Vertex),(char*) NULL+0); 
  glNormalPointer(GL_FLOAT, sizeof(Vertex),(char*) NULL+16); 
  glColorPointer(4,GL_FLOAT,  sizeof(Vertex),(char*) NULL+32);

  // Now we are ready to draw, using the triangle indices in the buffer 
  //
  // The following code uses GLM, the OpenGL mathematics library. This is to replace 
  // the OpenGL2.x matrix functions which have been deprecated. 
  // 
  // 
  // use the GLM helper functions to construct projection and modelview matrices 

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity(); 
  gluPerspective(60, 1, .1, 100); 

  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); 

  glLightfv(GL_LIGHT0, GL_POSITION, light_pos); 

  gluLookAt(0,0,5,0,0,0,0,1,0); 

  glRotatef(x_angle, 0, 1,0); 
  glRotatef(y_angle, 1,0,0); 
  glScalef(scale_size, scale_size, scale_size); 

  // the following transformation is for cylinder

  glTranslatef(0,0,-2.5); 
  glScalef(1,1,5); 

  glDrawElements(GL_TRIANGLE_STRIP, nindices, GL_UNSIGNED_INT, (char*) NULL+0); 

  glDisableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
  glDisableClientState(GL_COLOR_ARRAY); // enable the color array on the client side

  glutSwapBuffers(); 

} 

//////////////////////////////////////////////////////////////////////////////////
//
// create VBO objects and send the triangle vertices/colors to the graphics card
// 
void InitVBO(int nslices, int nstacks) 
{
  int nvertices = nslices * nstacks; 
  nindices = (nstacks-1)*2*(nslices+1); 

  glGenBuffers(1, vboHandle);   // create an interleaved VBO object
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);   // bind the first handle 

  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, cyverts, GL_STATIC_DRAW); // allocate space and copy the position data over
  glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up 

  glGenBuffers(1, &indexVBO); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, cindices, GL_STATIC_DRAW);  // load the index data 

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up 

  // by now, we moved the position and color data over to the graphics card. There will be no redundant data copy at drawing time 
} 
//////////////////////////////////////////////////////////////////////////////////
//
//    GLUT stuff 
//
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
	if (key == 'q') exit(1); 
	glutPostRedisplay(); 
}
///////////////////////////////////////////////////////////////////////////////
//
int main(int argc, char** argv) 
{ 
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
  glutInitWindowSize(600,600); 

  glutCreateWindow("shader cube"); 
  glutDisplayFunc(display); 
  glutMouseFunc(mymouse); 
  glutMotionFunc(mymotion);
  glutKeyboardFunc(mykey); 

  // initialize GLEW 

  GLenum err = glewInit(); 

  if ( err != GLEW_OK)  printf(" Error initializing GLEW! \n"); 
  else printf("Initializing GLEW succeeded!\n"); 

  // define the discretion level for the cylinder 
  int nslices, nstacks; 
  nslices = 60; 
  nstacks = 20; 

  InitCylinder(nslices, nstacks, 1.0, 0.0, 1.0); 
  InitVBO(nslices, nstacks); 

  glutMainLoop(); 
} 
