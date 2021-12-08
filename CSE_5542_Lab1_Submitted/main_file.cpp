#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glui.h>
#include <GL/glut.h> 
#include "primitive.h"

using namespace std;

const float PI = 3.14;

//Globar Vars
vector<primitive> list;
pointClass uv(1,0,1,1,1);
pointClass newPoint(0,0,1,1,1);
float angle=0;
int num=0;
float r=1.0f; 
float g=1.0f;
float b=1.0f;
float  D = 50;
int N=0; 
float S=0;
int width=600;
int height = 600;


void drawPolygon(float angle)
{
	int primNum = list.size()-1;

	for(int i=0;i<N;i++)
	{		
		pointClass yy = uv;
		pointClass temp;
		uv.x = yy.x*cos(angle)-yy.y*sin(angle);
		uv.y = yy.y*cos(angle)+yy.x*sin(angle);

		temp.x = list[primNum].points[list[primNum].points.size()-1].x + uv.x*D;
		temp.y = list[primNum].points[list[primNum].points.size()-1].y + uv.y*D;
		list[primNum].points[list[primNum].points.size()-1].color.r=r;
		list[primNum].points[list[primNum].points.size()-1].color.g=g;
		list[primNum].points[list[primNum].points.size()-1].color.b=b;

		list[primNum].points.push_back(temp);
	}

	N=0;
	S=0;
}

void drawSpiral(float angle)
{
	int primNum = list.size()-1;

	for(int i=0;i<N;i++)
	{		
		pointClass yy = uv;
		pointClass temp;
		uv.x = yy.x*cos(angle)-yy.y*sin(angle);
		uv.y = yy.y*cos(angle)+yy.x*sin(angle);

		uv.x = S*uv.x;
		uv.y = S*uv.y;

		temp.x = list[primNum].points[list[primNum].points.size()-1].x + uv.x*D;
		temp.y = list[primNum].points[list[primNum].points.size()-1].y + uv.y*D;
		list[primNum].points[list[primNum].points.size()-1].color.r=r;
		list[primNum].points[list[primNum].points.size()-1].color.g=g;
		list[primNum].points[list[primNum].points.size()-1].color.b=b;

		list[primNum].points.push_back(temp);
	}

	N=0;
	S=0;
}

void drawPolyLine()
{
	int i = list.size()-1;
	
	pointClass temp;
	temp.x = list[i].points[list[i].points.size()-1].x + uv.x*D;
	temp.y = list[i].points[list[i].points.size()-1].y + uv.y*D;
	list[i].points[list[i].points.size()-1].color.r=r;
	list[i].points[list[i].points.size()-1].color.g=g;
	list[i].points[list[i].points.size()-1].color.b=b;
	list[i].points.push_back(temp);	
}

void display() 
{ 
	glClearColor(0,0,0,1); 
	glClear(GL_COLOR_BUFFER_BIT); 
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,0,height);	

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	if(list.size()>0)
	{	
		glBegin(GL_LINES);
		for(int i=0;i<list.size();i++)
		{
			if(list[i].points.size()>1)
			{
				for(int j=0;j<list[i].points.size()-1;j++)
				{
					glColor4f(list[i].points[j].color.r,list[i].points[j].color.g,list[i].points[j].color.b,1);
					glVertex2f(list[i].points[j].x,list[i].points[j].y);					
					glVertex2f(list[i].points[j+1].x,list[i].points[j+1].y);
				}
			}
		}
		glEnd();
	}


	glutSwapBuffers();
} 

void mymouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		switch (state)
		{
		case GLUT_DOWN:	
			newPoint.x=x;
			newPoint.y=height-y; // to flip the y value and make openGL and GLUT consistent
			
			primitive p;
			colorClass c(r,g,b);
			newPoint.color.r=r;
			newPoint.color.g=g;
			newPoint.color.b=b;
			p.points.push_back(newPoint);	// add the initial point to vector for points		
			list.push_back(p); // push the primitive to array of primitives
			uv.x=1;	uv.y=0; // set the uv vector again to (0,1) for new primitive
			break;			
		}
	}

	glutPostRedisplay(); 
}


void mykey(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'q': // quit
		exit(1);

	case 'f': // draw lines
		drawPolyLine();
		break;

	case 'l': // scale up 50% of the length
		uv.x = 1.5*uv.x;
		uv.y = 1.5*uv.y;
		break;

	case 's': // scale down 50% of the length 
		uv.x = 0.5*uv.x;
		uv.y = 0.5*uv.y;
		break;

	case 't': // turn by the next line by some angle
		if(N==0) // N=0 means drawing poly lines part
		{
			pointClass xx = uv;
			uv.x = xx.x*cos(PI/3)-xx.y*sin(PI/3);
			uv.y = xx.y*cos(PI/3)+xx.x*sin(PI/3);
		}
		else // else drawing polygons part
		{
			pointClass xx = uv;
			uv.x = xx.x*cos(2*PI/N)-xx.x*sin(2*PI/N);
			uv.y = xx.y*cos(2*PI/N)+xx.x*sin(2*PI/N);
		}
		break;

	case 'p':		
		N=6;
		angle = 2*PI/N; 
		drawPolygon(angle);
		break;

	case 'a':		
		N=5;
		angle = 4*PI/N;
		drawPolygon(angle);
		break;

	case 'w':		

		N=25;
		S=0.9;
		angle = 6*PI/N;
		drawSpiral(angle);
		break;

	case 'r':
		r=1; g=0; b=0;
		break;

	case 'g':
		r=0; g=1; b=0;
		break;

	case 'b':
		r=0; g=0; b=1;
		break;

	case 'c':

		for(int i=0;i<list.size();i++)
		{
			list[i].points.clear();
		}
		break;
	}
	glutPostRedisplay(); 
}

void reshape(int w, int h)
{    
	float aspect = (float)w / (float)h;
	width = w;
    height = h;	
    	
    glViewport(0,0,aspect*width,aspect*height);
}


///////////////////////////////////////////////////////////////
// Main Function //
int main(int argc, char** argv) 
{ 

	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
	glutInitWindowSize(width,height); 

	glutCreateWindow("CSE 5542 Lab1"); 
	glutDisplayFunc(display); 
	glutIdleFunc(display);
	glutMouseFunc(mymouse); 
	glutKeyboardFunc(mykey); 
	glutReshapeFunc(reshape);

	glutMainLoop(); 

} 

