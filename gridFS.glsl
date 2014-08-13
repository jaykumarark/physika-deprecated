#version 330

//In Variables
in vec3 fVertexColor;
in vec3 fVertexNormal; 
in vec2 fVertexTexture; 
in vec3 fVertexPosition;

//Uniforms
uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;

uniform vec4 lightPosition;

uniform vec3 ka; 
uniform vec3 la; 

uniform vec3 kd; 
uniform vec3 ld; 

uniform vec3 ks; 
uniform vec3 ls; 

uniform sampler2D TextureSample2D;

//Out Variables
out vec4 outputColor;


void main()
{		
	vec3 tnorm = vec3(ModelViewMatrix * vec4(fVertexNormal, 0));
	vec4 eyeCoords = ModelViewMatrix * vec4(fVertexPosition, 1.0); 
	vec4 lightEye = ViewMatrix * lightPosition;
	vec3 s = normalize(vec3(lightEye - eyeCoords));
	vec3 r = reflect(-s, tnorm);
	vec3 v = normalize(-eyeCoords.xyz);

	float shininess = 1; 
	vec3 spec = vec3(0);

	float sdotn = max(dot(s, tnorm), 0.0);

	vec3 diffuseTerm = ld * kd * sdotn; 

	vec3 ambientTerm = la * ka;
	
	if(sdotn > 0.0)
		spec = ls*ks*pow(max(dot(r,v),0.0),shininess);
	
	vec4 texColor = texture(TextureSample2D, fVertexTexture);

	outputColor = vec4(diffuseTerm+ambientTerm, 1.f) * texColor + vec4(spec, 1.f);
}
