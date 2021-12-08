
varying vec4 pcolor; 
varying vec3 fNormal;
varying vec3 v; // position in world space
varying vec2 texCoordinate;

uniform float kamb;
uniform float kdiff;
uniform float kspec;
uniform int shininess;
uniform vec4 diffuseTerm;
uniform int L1;
uniform int L2;
uniform sampler2D reflectionMap;
uniform int isReflect;
uniform mat4 model; //passed to shader 
uniform mat4 projection; //passed to shader
uniform mat4 view; // passed to shader
uniform sampler2D ShadowMap; // this is depthTexture
uniform mat4 invMVPMat; // this matrix takes from screen to world space

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

void main() 
{  
   vec4  lightEye;
   vec3 norm,L,E,R;
   gl_FragColor = vec4(0,0,0,1);
   norm = normalize(fNormal);

   //Look up sceneTexture rendered in 2nd pass
   vec2 coord = gl_FragCoord.xy / vec2(800,800);
   vec4 reflectionPlaneColor = texture(reflectionMap,coord); // calculate color for texture 
   
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

   if(isReflect==1)
      gl_FragColor += Iamb + mix(reflectionPlaneColor,Idiff,0.3) + Ispec;  
   else
      gl_FragColor += Iamb + Idiff + Ispec; 
} 
