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
		vec3 v = normalize(view);	
		vec3 n = normalize(normal0);	//normal of the surface for each vertex
		vec3 s = normalize(light);  //direction light source. 
		vec3 r = reflect(-s, n);
		
		//ambient
		vec3 la = vec3(0.3, 0.3, 0.3);
		vec3 ka = vec3(0.3, 0.3, 0.3);
		vec3 ambient = la*ka;

		//diffuse
		vec3 ld = vec3(.5, 0.5, 0.5); // color of light with intensity for each component
		float kd = vec3(0.8, 0.2, 0.5); //diffuse reflectivity
		vec3 diffuseTerm = ld * kd * max(dot(s, n), 0.0);

		//specular
		vec3 ls = vec3(1,1,1);
		vec3 ks = vec3(.9, 0.9, 0.9);
		vec3 spec = ls * ks * pow(max(dot(r, v), 0.0), 50);


		//outputColor = color0 * diffuse;
		outputColor = vec4(diffuseTerm+ambient+spec, 1.0);
}
