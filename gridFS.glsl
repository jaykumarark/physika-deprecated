#version 330

in vec4 color0;
in vec3 normal0; 
in vec2 texcoord0; 
in vec3 light;
in vec3 view;

out vec4 outputColor;
uniform sampler2D gtexture;

void main()
{		
	outputColor = color0;
}
