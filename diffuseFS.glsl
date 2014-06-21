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
	const float scale = 15.0;
	bvec2 toDiscard = greaterThan(fract(texcoord0*scale), vec2(0.1, 0.1));

	if(all(toDiscard))
		discard;


	outputColor = color0;
	//outputColor = vec4(1, 0, 0, 1);
}
