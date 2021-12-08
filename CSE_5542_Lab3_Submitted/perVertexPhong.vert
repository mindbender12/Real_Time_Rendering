attribute vec4 position; 
attribute vec4 color1; 
attribute vec4 normal; 

uniform mat4 model; //passed to shader 
uniform mat4 projection; //passed to shader
uniform mat4 view; // passed to shader
uniform mat4 normalMatrix; //passed to shader
uniform vec4 diffuseTerm;
uniform float kamb;
uniform float kdiff;
uniform float kspec;
uniform int shininess;
uniform int L1;
uniform int L2;

varying vec4 pcolor; 

vec3 fNormal;
vec3 v;
vec4 Iamb=vec4(0,0,0,1);
vec4 Idiff=vec4(0,0,0,1);
vec4 Ispec=vec4(0,0,0,1);

struct Lights {
vec4 position;
vec4 color;
float intensity;
};

Lights light1 = Lights(vec4(-60,55,80,0),vec4(0,1,0,1),0.8);
Lights light2 = Lights(vec4(80,-10,40,0),vec4(1,0,0,1),0.7);

void main()
{     
      pcolor = vec4(0,0,0,1);
      vec4  lightEye;
      vec3 norm,L,E,R;
      mat4 projectionModelView;      
      vec4 N=normalMatrix*normal;       
	  fNormal = vec3(N); 
	  norm = normalize(fNormal);
	  projectionModelView=projection*view*model;	  
      gl_Position =  projectionModelView * position; // calculate clip space position	  
      v=vec3(view*model*position); // v is the position at eye space for each vertex 
	  norm = normalize(fNormal);
	  
	  //For light 1
   lightEye = view*light1.position; //converting light to eye space    
   L = normalize(vec3(lightEye) - v);   
   E = normalize(-v); 
   R = normalize(-reflect(L,norm));

   //calculate Phong Term:  
   Iamb = light1.intensity*diffuseTerm*kamb;    
   if(L1==1)
   { 
   Idiff = light1.intensity*diffuseTerm*(kdiff * max(dot(norm,L), 0.0));    
   Ispec = vec4(1,1,1,1)* pow(max(dot(R,E),0.0),shininess)*kspec;
   }
   
   pcolor += Iamb + Idiff + Ispec;
   
   //For light 2
   lightEye = view*light2.position; //converting light to eye space     
   L = normalize(vec3(lightEye) - v);   
   E = normalize(-v); 
   R = normalize(-reflect(L,norm));

   //calculate Phong Term:  
   Iamb = light2.intensity*diffuseTerm*kamb;  
   if(L2==1)
   {  
   Idiff = light2.intensity*diffuseTerm*(kdiff * max(dot(norm,L), 0.0));    
   Ispec = vec4(1,1,1,1)* pow(max(dot(R,E),0.0),shininess)*kspec;
   }
    
    pcolor += Iamb + Idiff + Ispec;
}