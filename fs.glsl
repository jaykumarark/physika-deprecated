#version 330

in vec4 f_color;
out vec4 outputColor;
uniform sampler2D tex;
void main()
{		
		vec4 dyncolor = vec4(gl_FragCoord.y, gl_FragCoord.x, gl_FragCoord.z, 1.0);
		outputColor = f_color;
		//outputColor = texture(tex, gl_PointCoord)*dyncolor;
}
