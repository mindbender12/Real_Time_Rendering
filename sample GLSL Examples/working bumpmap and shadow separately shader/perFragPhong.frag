
varying vec4 pcolor; 
varying vec3 fNormal;
varying vec3 v; // position in world space
varying mat4 modelMat;
varying mat4 viewMat;
varying mat4 projectionMat;
varying vec2 texCoordinate;
varying vec4 shadowTextureCoord;

uniform float kamb;
uniform float kdiff;
uniform float kspec;
uniform int shininess;
uniform vec4 diffuseTerm;
uniform int L1;
uniform int L2;
uniform sampler2D Tex1;
uniform sampler2D ShadowMap;
uniform int needTexture;
uniform mat4 view;

vec4 Iamb=vec4(0,0,0,1);
vec4 Idiff=vec4(0,0,0,1);
vec4 Ispec=vec4(0,0,0,1);

struct Lights 
{
vec4 position;
vec4 color;
float intensity;
};

Lights light1 = Lights(vec4(50,90,100,0),vec4(0,1,0,1),0.8);

void main() 
{    
   vec4  lightEye;
   vec3 norm,L,E,R;   
   norm = normalize(fNormal);
   gl_FragColor = vec4(0,0,0,1);
   
  //Normal texture Lookup
   vec4 texColor = texture(Tex1,texCoordinate); // calculate color for texture
   //float shadow = shadow2DProj(ShadowMap, shadowTextureCoord).x;


  float shadowFactor = 1.0; // default to '1' meaning "no shadow"
  float epsilon = 0.002; // increase value to remove little artifacts
  vec4 shadCoordsPD = shadowTextureCoord / shadowTextureCoord.w;
  if (shadowTextureCoord.w <= 0.0f) { // ignore negative projection
    shadowFactor = 1;
  } else if (shadCoordsPD.x < 0 || shadCoordsPD.y < 0) { // outside light frustum, ignore
    shadowFactor = 1;
  } else if (shadCoordsPD.x >= 1 || shadCoordsPD.y >= 1) { // outside light frustum, ignore
    shadowFactor = 1;
  } else {
    vec4 shadow = texture(ShadowMap, shadCoordsPD.xy);
    if (shadow.z + epsilon < shadCoordsPD.z) {
      shadowFactor = 0.0;
    }
  }


   //Phong Light Calculation
      
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
        gl_FragColor += Iamb + texColor + Ispec; 
     }
   else
       gl_FragColor += Iamb + Idiff + Ispec; 

	 gl_FragColor = shadowFactor*gl_FragColor+Iamb;
   }
   
