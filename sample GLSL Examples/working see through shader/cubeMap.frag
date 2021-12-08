

varying vec3 fNormal;
varying vec3 v; // position in world space
varying vec2 texCoordinate;
varying vec3 reflector;

uniform float kamb;
uniform float kdiff;
uniform float kspec;
uniform int shininess;
uniform vec4 diffuseTerm;
uniform int L1;
uniform int L2;
uniform samplerCube cubeMap;
uniform sampler2D Tex1;
uniform int needTexture;
uniform int isCubeMap;
uniform mat4 view; 
uniform int isReflect;
uniform sampler2D reflectionMap;

vec4 Iamb=vec4(0,0,0,1);
vec4 Idiff=vec4(0,0,0,1);
vec4 Ispec=vec4(0,0,0,1);

struct Lights 
{
vec4 position;
vec4 spotDir;
float intensity;
};

Lights light1 = Lights(vec4(-20,80,300,0),normalize(vec4(0,80,0,1)-vec4(-20,80,300,0)),0.8); // spotDir = normalize(COI-lightLoc);

//Generate 
float rand(vec2 co){
    return fract(cos(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() 
{  
   vec4  lightEye;
   vec3 norm,L,E,R;
   gl_FragColor = vec4(0,0,0,1);
   norm = normalize(fNormal);
   vec2 coord;

   coord = gl_FragCoord.xy / vec2(800,800) + rand(texCoordinate)/80;   
   vec4 reflectionPlaneColor = texture(reflectionMap,coord); // calculate color for texture 
    
   vec4 cubeMapColor = textureCube(cubeMap,reflector); //calculate color for Cube Map
   vec4 texColor = texture(Tex1,texCoordinate); //calculate color for 2D texture   
   
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
   Ispec = vec4(1,1,1,1)* pow(max(dot(R,E),0.0),shininess)*kspec; // vec4(1,1,1,1) because specular is always white light dependent 
   }

   if(needTexture==1)
      {
	     if(isCubeMap==1 && isReflect==1)
		    gl_FragColor += Iamb + mix(cubeMapColor,reflectionPlaneColor,0.6) + Ispec; 
		else if(isCubeMap==1 && isReflect==0)
		    gl_FragColor += Iamb + cubeMapColor + Ispec; 
		else
			gl_FragColor += Iamb + texColor + Ispec; 
	 }
   else
      gl_FragColor += Iamb + Idiff + Ispec;  

 } 
