#version 330

//In variables
in vec3 VertexPosition; 
in vec3 VertexNormal;
in vec2 VertexTexture; 
in vec3 VertexColor;


//Out Variables
out vec3 fVertexPosition;
out vec3 fVertexNormal;
out vec2 fVertexTexture;
out vec3 fVertexColor;


//Matrices
uniform mat4 mvp;
void main()
{
	fVertexPosition = VertexPosition; 
	fVertexNormal   = VertexNormal; 
	fVertexTexture  = VertexTexture; 
	fVertexColor    = VertexColor; 
	gl_Position =	mvp * vec4(VertexPosition, 1.0f);
}