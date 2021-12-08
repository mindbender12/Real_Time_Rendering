
varying vec4 pcolor; 
varying vec3 fNormal;
varying vec3 v; // position in world space
varying vec2 texCoordinate;
varying vec4 shadowTextureCoord;
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
uniform sampler2D ShadowMap;

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
	vec3  lightEye;
	vec3 norm,L,E,R;   
	vec3 vertexPos=v;
	gl_FragColor = vec4(0,0,0,1);   

	//Normal texture Lookup
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

	//Phong Light Calculation

	//For light 1    
	if(isBumpMap==0)
	{
		lightEye =vec3(view*light1.position);
	}
	else if (isBumpMap==1)
	{
		lightEye = tangentSpace*vec3(light1.position);
	} 

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

	if(needTexture==1 && isBumpMap==0)
		gl_FragColor += Iamb + texColor + Ispec; 
   	else if(needTexture==1 && isBumpMap==1)
		gl_FragColor += Iamb + Idiff*texColor + Ispec; 
	else
		gl_FragColor += Iamb + Idiff + Ispec; 

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
	
}

