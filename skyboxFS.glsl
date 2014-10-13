#version 330

//In Variables
in vec3 fVertexColor;
in vec3 fVertexNormal; 
in vec2 fVertexTexture; 
in vec3 fVertexPosition;

uniform samplerCube TextureSample3D;

out vec4 frag_color; 

void main()
{
    frag_color = texture(TextureSample3D, fVertexPosition);
}