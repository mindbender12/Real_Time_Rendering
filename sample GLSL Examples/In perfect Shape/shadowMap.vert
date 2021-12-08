
attribute vec4 position; 
attribute vec4 color1; 
attribute vec4 normal; 
attribute vec2 texCoord;

uniform mat4 model; //passed to shader 
uniform mat4 projection; //passed to shader
uniform mat4 view; // passed to shader
uniform mat4 normalMatrix; //passed to shader
uniform mat4 lightViewMatrix;
uniform sampler2D normal_texture;

const mat4 biasMat = mat4(0.5, 0.0, 0.0, 0.0,
                       0.0, 0.5, 0.0, 0.0,
                       0.0, 0.0, 0.5, 0.0,
                       0.5, 0.5, 0.5, 1.0);


varying vec4 pcolor; 
varying vec3 fNormal;
varying vec3 v;
varying vec2 texCoordinate;
varying vec4 shadowTextureCoord;
varying mat3 tangentSpace;

void main()
{     
      texCoordinate = texCoord;
      mat4 projectionModelView;
      vec4 N;     
      N=normalMatrix*normal;	  
      fNormal = vec3(N);//need to multiply this with normal matrix
	  projectionModelView=projection*view*model;
	  v=vec3(view*model*position); // v is the position at eye space for each vertex passed to frag shader
	  mat4 lightprojectionModelView = biasMat*lightViewMatrix*model;
	  shadowTextureCoord = lightprojectionModelView*position;
      gl_Position =  projectionModelView * position; // calculate clip space position

	   //Form TBN space
	   vec3 tangent   = vec3(normalize(normalMatrix * texture(normal_texture,texCoord)));	
       vec3 binormal  = cross(fNormal, tangent);
       tangentSpace = mat3(tangent, binormal, fNormal);  

}