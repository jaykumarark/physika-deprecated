#version 330

in vec3 position; 
in vec2 texcoord; 
in vec3 normal;
in vec3 color;

uniform mat4 projection;
uniform vec3 lightpos;

out vec2 texcoord0;
out vec3 normal0 ;
out vec4 color0;
out vec3 light;
out vec3 view;

void main()
{
	gl_Position = projection * vec4(position, 1.0f);
	light = position-vec3(200, 500, -300);
	texcoord0 = texcoord; 
	normal0 = normal;
	vec3 view = -gl_Position.xyz;
	color0 = vec4(color, 1.0);
}