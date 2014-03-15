#version 330

in vec4 color0;
in vec3 normal0; 
in vec2 texcoord0; 
in vec3 light;


out vec4 outputColor;
uniform sampler2D gtexture;

void main()
{		
		/*vec4 diffuseTerm = vec4(1.0, 1.0, 1.0, 1.0); //* vec4(1.0, 1.0, 1.0, 1.0);
		vec3 n = normalize(normal0);
		vec3 l = normalize(light);
		float kd = max(dot(l, n), 0.0); 
		vec4 diffuse = kd * diffuseTerm;
		outputColor = color0 + diffuse;*/

		outputColor = vec4(1,0,0,1);
}
