#version 330

//In Variables
in vec3 VertexPosition; 
in vec3 VertexColor; 


//Matrices
uniform mat4 mvp; 

void main()
{
	gl_Position = mvp * vec4(VertexPosition, 1.0f);
}