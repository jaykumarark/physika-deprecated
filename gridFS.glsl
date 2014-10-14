#version 330

//In Variables
in vec3 fVertexColor;
in vec3 fVertexNormal; 
in vec2 fVertexTexture; 
in vec3 fVertexPosition;

//Uniforms
uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;

uniform vec4 lightPosition;
uniform vec3 EyePositionInWorld;

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
	
	vec4 VertexEyeCoords = ModelViewMatrix * vec4(fVertexPosition, 1.0); 
	vec4 lightEye = ViewMatrix * lightPosition;
	vec3 lightVector = normalize(vec3(lightEye - VertexEyeCoords));
	
	
	//Surface normal vector
	vec3 tnorm = normalize(vec3(NormalMatrix * fVertexNormal));

	//Reflect vector
	vec3 r = reflect(-lightVector, tnorm);
	vec3 v = normalize(EyePositionInWorld-VertexEyeCoords.xyz);

	float shininess = 10; 
	vec3 spec = vec3(0);

	float sdotn = max(dot(lightVector, tnorm), 0.0);

	vec3 diffuseTerm = ld * kd * sdotn; 

	vec3 ambientTerm = la * ka;
	
	if(sdotn > 0.0)
		spec = ls*ks*pow(max(dot(r,v),0.0),shininess);
	
	vec4 texColor = texture(TextureSample2D, fVertexTexture);

	outputColor = vec4(ambientTerm+diffuseTerm+spec, 1.f);
}
