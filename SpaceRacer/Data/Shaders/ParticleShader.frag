#version 120
uniform sampler2D u_Texture;

varying vec4 v_Color;

void main()
{
	gl_FragColor = v_Color;
}
