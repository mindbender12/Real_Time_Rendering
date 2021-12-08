uniform sampler2D Tex0, Tex1;
varying float ClipDist;
varying vec3 ecPos, lightDir;
varying vec3 normal; // XXX: debug

void main(void)
{
	vec3 n, l, r;
	float NdotL, RdotE;
	float dist, att;
	vec2 TexCoord;
	float height, scale = 0.025, bias = 0.0125;

	if (ClipDist < 0.0)
		discard;

	height = scale * texture2D(Tex1, gl_TexCoord[0].st).a - bias;
	TexCoord = gl_TexCoord[0].st + height * normalize(-ecPos).xy;
//	TexCoord = gl_TexCoord[0].st;


	vec4 normalMap = texture2D(Tex1, TexCoord);
	vec4 colorMap = texture2D(Tex0, TexCoord);

	n = normalize(2.0 * normalMap.rgb - 1.0); // decoding normal map
//	n = normalize(normal);

	dist = length(lightDir);
	l = normalize(lightDir);

	att = 1.0 / (gl_LightSource[0].constantAttenuation +
                 gl_LightSource[0].linearAttenuation * dist +
                 gl_LightSource[0].quadraticAttenuation * dist * dist);

	gl_FragColor = gl_LightSource[0].ambient * att;

	NdotL = max(dot(n, l), 0.0);

	if (NdotL > 0.0) {
		gl_FragColor += att * (gl_LightSource[0].diffuse * NdotL);
		r = reflect(-l, n);
		RdotE = max(dot(r, normalize(-ecPos)), 0.0);
		gl_FragColor += att * gl_LightSource[0].specular * pow(RdotE, 100.0);
	}

	gl_FragColor *= colorMap;
}
