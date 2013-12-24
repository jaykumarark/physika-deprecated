#version 330

in vec3 position; 
in vec4 color;
out vec4 f_color;
uniform mat4 projection;
void main()
{
	gl_Position = projection * vec4(position, 1.0f);
	f_color = color;
	
}