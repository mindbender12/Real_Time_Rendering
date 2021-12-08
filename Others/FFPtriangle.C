
//////////////////////////////////////////////////////// 
// 
// This is a very simple OpenGL/Glut program using fixed function pipeline (FFP) 
// Although the fixed function pipeline has been slowly replaced with shaders, and many functions used 
// in this program have been deprecated, 
// the purpose of this program is to show how a simple OpenGL program can be constructed
//
// I believe that the simplicity of fixed function pipeline OpenGL program makes it easier for students 
// to understand the high level concept of graphics programming. Also since the FFP OpenGL programs 
// will not just go away overnight, students should been able to read and understand those programs. 
//
// Han-Wei Shen, July 2012 @ Ohio State University 
// hwshen@cse.ohio-state.edu 
// 
//////////////////////////////////////////////////////// 

#include <GLUT/glut.h> 
#include <OpenGL/gl.h> 
#include <stdio.h>
#include <stdlib.h>



float vertices[] = {-0.5, -0.5, 0.0, 1.0,  // first triangle
		    -0.5, 0.5, 0.0, 1.0, 
		    0.5, 0.5, 0.0, 1.0, 
		    0.5, 0.5, 0.0, 1.0,    // second triangle 
		    0.5, -0.5, 0.0, 1.0, 
		    -0.5, -0.5, 0.0, 1.0}; 


void display() 
{ 
  glClearColor(0,0,1,1); 
  glClear(GL_COLOR_BUFFER_BIT); 
  glColor4f(1,1,0,1);     //glColor* have been deprecated in OpenGL 3

  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); 
  glRotatef(45, 0,0,1); 

  glBegin(GL_TRIANGLES);  //glBegin/End have been deprecated in OpenGL 3

  glColor4f(1,1,0,1); 
  glVertex4f(vertices[0], vertices[1], vertices[2], vertices[3]); 
  glVertex4f(vertices[4], vertices[5], vertices[6], vertices[7]); 
  glVertex4f(vertices[8], vertices[9], vertices[10], vertices[11]); 

  glColor4f(1,0,0,1); 
  glVertex4f(vertices[12], vertices[13], vertices[14], vertices[15]); 
  glVertex4f(vertices[16], vertices[17], vertices[18], vertices[19]); 
  glVertex4f(vertices[20], vertices[21], vertices[22], vertices[23]); 

  glEnd(); 

  glutSwapBuffers(); 

} 

void mymouse(int button, int state, int x, int y)
{

  if (state == GLUT_DOWN)
    printf(" click on [%d, %d]!\n", x,y); 
  else if (state == GLUT_UP) 
    printf(" mouse button up! \n"); 

  glutPostRedisplay(); 
}

///////////////////////////////////////////////////////////////

void mykey(unsigned char key, int x, int y)
{
	if (key == 'q') exit(1); 
	glutPostRedisplay(); 
}

int main(int argc, char** argv) 
{ 

  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
  glutInitWindowSize(600,600); 

  glutCreateWindow("fixed function pipeline: simple"); 
  glutDisplayFunc(display); 
  glutMouseFunc(mymouse); 
  glutKeyboardFunc(mykey); 

  glutMainLoop(); 

} 
