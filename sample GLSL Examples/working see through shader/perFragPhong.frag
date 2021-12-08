
varying vec4 pcolor; 
varying vec3 fNormal;
varying vec3 v; // position in world space
varying mat4 modelMat;
varying mat4 viewMat;
varying mat4 projectionMat;
varying vec2 texCoordinate;


uniform float kamb;
uniform float kdiff;
uniform float kspec;
uniform int shininess;
uniform vec4 diffuseTerm;
uniform int L1;
uniform int L2;
uniform sampler2D reflectionMap;
uniform sampler2D Tex1;
uniform int needTexture;
uniform int isReflect;

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
   vec2 coord;
   gl_FragColor = vec4(0,0,0,1);
   norm = normalize(fNormal);
      
   coord = gl_FragCoord.xy / vec2(800,800);// + rand(texCoordinate)/80;
   
   vec4 reflectionPlaneColor = texture(reflectionMap,coord); // calculate color for texture 
   vec4 texColor = texture(Tex1,texCoordinate); //calculate color for 2D texture
   
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

   if(isReflect==1 && needTexture==1)
      gl_FragColor += Iamb + mix(reflectionPlaneColor,Idiff,0.4) + Ispec;  
   else if(isReflect==0 && needTexture==1 )
      gl_FragColor += Iamb + texColor + Ispec;   
  else
	  gl_FragColor += Iamb + Idiff + Ispec; 
 } 
