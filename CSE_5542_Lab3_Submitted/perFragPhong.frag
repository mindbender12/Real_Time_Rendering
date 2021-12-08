
varying vec4 pcolor; 
varying vec3 fNormal;
varying vec3 v; // position in world space
varying mat4 modelMat;
varying mat4 viewMat;
varying mat4 projectionMat;

uniform float kamb;
uniform float kdiff;
uniform float kspec;
uniform int shininess;
uniform vec4 diffuseTerm;
uniform int L1;
uniform int L2;

vec4 Iamb=vec4(0,0,0,1);
vec4 Idiff=vec4(0,0,0,1);
vec4 Ispec=vec4(0,0,0,1);

struct Lights 
{
vec4 position;
vec4 color;
float intensity;
};

Lights light1 = Lights(vec4(-60,55,80,0),vec4(0,1,0,1),0.8);
Lights light2 = Lights(vec4(80,-10,40,0),vec4(1,0,0,1),0.7);

void main() 
{  
   vec4  lightEye;
   vec3 norm,L,E,R;
   gl_FragColor = vec4(0,0,0,1);
   norm = normalize(fNormal);
   
    //For light 1     
   lightEye = viewMat*light1.position; //converting light to eye space    
   L = normalize(vec3(lightEye) - v);   
   E = normalize(-v); 
   R = normalize(-reflect(L,norm));

   //calculate Phong Term:  
   Iamb = light1.intensity*diffuseTerm*kamb;   
   if(L1==1)
   {  
   Idiff = light1.intensity*diffuseTerm*(kdiff * max(dot(norm,L), 0.0));    
   Ispec = vec4(1,1,1,1)* pow(max(dot(R,E),0.0),shininess)*kspec; // vec4(1,1,1,1) because specular is always white light dependent 
   }
   gl_FragColor += Iamb + Idiff + Ispec;   
   
   //For light 2   
   lightEye = viewMat*light2.position; //converting light to eye space   
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
    gl_FragColor += Iamb + Idiff + Ispec;
	
 } 
