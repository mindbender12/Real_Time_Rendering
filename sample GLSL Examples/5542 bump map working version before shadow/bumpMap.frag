
varying vec4 pcolor; 
varying vec3 fNormal;
varying vec3 v; // position in world space
varying vec2 texCoordinate;
varying mat3 tangentSpace;

uniform float kamb;
uniform float kdiff;
uniform float kspec;
uniform int shininess;
uniform vec4 diffuseTerm;
uniform int L1;
uniform int L2;
uniform sampler2D Tex1;
uniform int needTexture;
uniform mat4 model; //passed to shader 
uniform mat4 projection; //passed to shader
uniform mat4 view; // passed to shader
uniform mat4 normalMatrix; //passed to shader
uniform sampler2D normal_texture;
uniform int isCubeMap;
uniform int isBumpMap; 

vec4 Iamb=vec4(0,0,0,1);
vec4 Idiff=vec4(0,0,0,1);
vec4 Ispec=vec4(0,0,0,1);

struct Lights 
{
vec4 position;
vec4 color;
float intensity;
};

Lights light1 = Lights(vec4(0,80,160,0),vec4(0,1,0,1),0.8);

vec3 vertexTspace;

void main() 
{     
   vec3  lightEye;
   vec3 norm,L,E,R;
   gl_FragColor = vec4(0,0,0,1);
   vec3 vertexPos=v;
     
   vec4 texColor = texture(Tex1,texCoordinate); // calculate color for texture   
   vec4 newNormal = texture(normal_texture,texCoordinate); // calculate color for texture 

   if(isBumpMap==0)
   {
   norm = normalize(fNormal);
   vertexPos=v;
   }
   else if (isBumpMap==1)
   {
   norm = normalize(vec3(newNormal)-0.5); 
   vertexPos = tangentSpace*vec3(v);
   }  
      
    //For light 1    
  if(isBumpMap==0)
   {
   lightEye =vec3(view*light1.position);
   }
   else if (isBumpMap==1)
   {
   lightEye = tangentSpace*vec3(light1.position);
   }     
    
    L = normalize(vec3(lightEye) - vertexPos);   
    E = normalize(-vertexPos); 
    R = normalize(-reflect(L,norm));

   //calculate Phong Term:  
   Iamb = light1.intensity*diffuseTerm*kamb;   
   if(L1==1)
   {  
   Idiff = light1.intensity*diffuseTerm*(kdiff * max(dot(norm,L), 0.0));    
   Ispec = vec4(1,1,1,1)* pow(max(dot(R,E),0.0),shininess)*kspec; // vec4(1,1,1,1) because specular is always white light dependent 
   }

   if(needTexture==1 && isBumpMap==0)
   gl_FragColor += Iamb + texColor + Ispec; 
   else if(needTexture==1 && isBumpMap==1)
   gl_FragColor += Iamb + Idiff*texColor + Ispec; 
   else
   gl_FragColor += Iamb + Idiff + Ispec; 
  }
