#include "color.h"

class pointClass
{
public:
	float x;
	float y;
	colorClass color;

	pointClass();
	pointClass(float a,float b, float red, float green, float blue);
};

pointClass::pointClass()
{
	x=0; y=0;
	color.r=1;
    color.g=1;
	color.b=1;
}

pointClass::pointClass(float a,float b, float red, float green, float blue)
{
	x=a;
	y=b;
	color.r = red;
	color.g = green;
	color.b = blue;
}