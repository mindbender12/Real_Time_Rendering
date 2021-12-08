
attribute vec4 position; 
attribute vec4 color1; 
attribute vec4 normal; 

uniform mat4 model; //passed to shader 
uniform mat4 projection; //passed to shader
uniform mat4 view; // passed to shader
uniform mat4 normalMatrix; //passed to shader


varying vec4 specular;
varying vec4 pcolor; 
varying vec3 fNormal;
varying vec3 v;
varying mat4 modelMat;
varying mat4 viewMat;
varying mat4 projectionMat;


void main()
{     
      mat4 projectionModelView;
      vec4 N;
      projectionMat = projection;
      modelMat=model;
	  viewMat=view;
      N=normalMatrix*normal;      
	  fNormal = vec3(N);//need to multiply this with normal matrix
	  projectionModelView=projection*view*model;
	  v=vec3(view*model*position); // v is the position at eye space for each vertex passed to frag shader
      gl_Position =  projectionModelView * position; // calculate clip space position
}
