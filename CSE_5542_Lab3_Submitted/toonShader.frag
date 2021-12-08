
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

vec3 lightPos= vec3(-20,15,20); // light Position in world space
vec4 Iamb=vec4(0,0,0,1);
vec4 Idiff=vec4(0,0,0,1);
vec4 Ispec=vec4(0,0,0,1);

void main() 
{   
   vec4 light = viewMat*vec4(lightPos,0); //converting light to eye space   
   vec3 norm = normalize(fNormal);
   vec3 L = normalize(vec3(light) - v);   
   vec3 E = normalize(-v); 
   vec3 R = normalize(-reflect(L,norm));
   float intensity = max(dot(norm,L),0);
    gl_FragColor = vec4(0,0,0,1);
  
   //calculate Ambient Term:  
   Iamb = diffuseTerm*kamb; 
   
   if(L1==1)
   {
   //calculate diffuse Term:    
   if(intensity>0.95)   
    Idiff = diffuseTerm*(kdiff * max(dot(norm,L), 0.0)); 	 
	else if(intensity>0.5)   
    Idiff = 0.3*diffuseTerm*(kdiff * max(dot(norm,L), 0.0));  	  
	else if (intensity>0.25)  
    Idiff = 0.1*diffuseTerm*(kdiff * max(dot(norm,L), 0.0));
   else   
    Idiff = 0.01*diffuseTerm*(kdiff * max(dot(norm,L), 0.0)); 

   //calculate Specular Term:
      if(intensity>0.95)   
    Ispec = 0.8*vec4(1,1,1,1)* pow(max(dot(R,E),0.0),shininess)*kspec; 	
	else if(intensity>0.5)   
    Ispec = 0.3*vec4(1,1,1,1)* pow(max(dot(R,E),0.0),shininess)*kspec;  
	else if (intensity>0.25)  
    Ispec = 0.1*vec4(1,1,1,1)* pow(max(dot(R,E),0.0),shininess)*kspec;
   else   
    Ispec = 0.01*vec4(1,1,1,1)* pow(max(dot(R,E),0.0),shininess)*kspec;
	}

   
    gl_FragColor = Iamb;
	gl_FragColor += Idiff;
	gl_FragColor += Ispec;
 } 
