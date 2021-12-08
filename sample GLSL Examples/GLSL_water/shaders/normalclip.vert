attribute vec4 Tangent; // handedness in w
uniform vec4 ClipPlane;
varying float ClipDist;
varying vec3 ecPos, lightDir;
varying vec3 normal; // XXX: debug

void main(void)
{
	ecPos = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	vec3 tangent = normalize(gl_NormalMatrix * Tangent.xyz);
	vec3 binormal = Tangent.w * cross(normal, tangent);

	// column-major matrix
	mat3 tbnMatrix = mat3(tangent.x, binormal.x, normal.x,
                          tangent.y, binormal.y, normal.y,
                          tangent.z, binormal.z, normal.z);

	lightDir = gl_LightSource[0].position.xyz - ecPos;
	lightDir = tbnMatrix * lightDir;
	ecPos = tbnMatrix * ecPos;
	normal = tbnMatrix * normal; // XXX: debug

	ClipDist = dot(gl_Vertex.xyz, ClipPlane.xyz) + ClipPlane.w;
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}
