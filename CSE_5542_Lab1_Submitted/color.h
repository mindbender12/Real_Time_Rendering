class colorClass
{
public:
	float r;
	float g;
	float b;

	colorClass();
	colorClass(float a,float b, float c);
};

colorClass::colorClass()
{
	r=1; g=1; b=1;
}

colorClass::colorClass(float red,float green, float blue)
{
	r=red;
	g=green;
	b=blue;
}