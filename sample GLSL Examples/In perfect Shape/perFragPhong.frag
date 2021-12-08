
varying vec4 pcolor; 
varying vec3 fNormal;
varying vec3 v; // position in world space
varying vec2 texCoordinate;
varying vec4 shadowTextureCoord;

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
uniform sampler2D ShadowMap;
uniform mat4 model; //passed to shader 
uniform mat4 projection; //passed to shader
uniform mat4 view; // passed to shader
uniform int isPly;
uniform sampler2D reflectionTexture;
uniform int needReflection;

vec4 Iamb=vec4(0,0,0,1);
vec4 Idiff=vec4(0,0,0,1);
vec4 Ispec=vec4(0,0,0,1);

//Generate 
float rand(vec2 co){
    return fract(cos(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

struct Lights 
{
vec4 position;
vec4 spotDir;
float intensity;
};

Lights light1 = Lights(vec4(-20,80,450,0),normalize(vec4(0,80,0,1)-vec4(-20,80,300,0)),0.8); // spotDir = normalize(COI-lightLoc);

void main() 
{  
   vec4  lightEye;
   vec3 norm,L,E,R;
   vec2 coord;
   vec2 refCoord;
   gl_FragColor = vec4(0,0,0,1);
   norm = normalize(fNormal);
     
   //Refraction texture lookup 
   coord = (gl_FragCoord.xy) / vec2(800,800);
   coord.y = coord.y-0.05;

   //Reflection texture lookup
   refCoord = (gl_FragCoord.xy) / vec2(800,800) + rand(texCoordinate)/100;
   //refCoord.y = coorrefCoordd.y-0.05;
   
   vec4 reflectionPlaneColor = texture(reflectionMap,coord); // calculate color for texture 
   vec4 texColor = texture(Tex1,texCoordinate); //calculate color for 2D texture
   vec4 actualReflectionColor = texture(reflectionTexture,refCoord); // calculate color for texture 
   
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

  if(isReflect==1 && needTexture==1)
      gl_FragColor += Iamb + mix(reflectionPlaneColor,Idiff,0.5) + Ispec;  
  else  if(isReflect==1 && needTexture==0)
      gl_FragColor += Iamb + mix(reflectionPlaneColor,Idiff,0.8) + Ispec;  
  else if(isReflect==0 && needTexture==1 )
      gl_FragColor += Iamb + texColor + Ispec;   
  else
	  gl_FragColor += Iamb + Idiff + Ispec; 

  if(isPly==1)
     gl_FragColor = Iamb + Idiff + Ispec;


	  //************************** Shadow Calculation Here ********************************************* //

	float shadowFactor = 1.0; // default to '1' meaning "no shadow"
	float epsilon = 0.002; // increase value to remove little artifacts

	if (shadowTextureCoord.w <= 0.0f)  // ignore negative projection
	{
		shadowFactor = 1;
	} 
	else
	{
		vec4 shadCoord = shadowTextureCoord/shadowTextureCoord.w;

		if (shadCoord.x < 0 || shadCoord.y < 0) // outside light frustum, ignore
		{ 
			shadowFactor = 1;
		}
		else if (shadCoord.x >= 1 || shadCoord.y >= 1 ) // outside light frustum, ignore
		{ 
			shadowFactor =1;
		} 
		else {
			vec4 shadow = texture(ShadowMap, shadCoord.xy);
			if (shadow.x + epsilon < shadCoord.z && shadow.y + epsilon < shadCoord.z)
			{
				shadowFactor = 0.3;
			}
		}
	}

	gl_FragColor = shadowFactor*gl_FragColor;

	if(needReflection==1)
	    gl_FragColor = mix(gl_FragColor,actualReflectionColor,0.6);
} 
